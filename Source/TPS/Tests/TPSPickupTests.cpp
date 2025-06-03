#if (WITH_AUTOMATION_TESTS)

#include "Components/SphereComponent.h"
#include "Components/TPSInventoryComponent.h"
#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "TPS/Items/TPSBasePickup.h"
#include "TPSCharacter.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated,
                                 "TPSGame.Items.Inventory.CppActorCantBeCreated",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly,
                                 "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupDataShouldBeSetupCorrectly,
                                 "TPSGame.Items.Inventory.PickupDataShouldBeSetupCorrectly",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCanBeTaken,
                                 "TPSGame.Items.Inventory.PickupCanBeTaken",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryPickupMeshExist,
                                 "TPSGame.Items.Inventory.EveryPickupMeshExist",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)


using namespace TPS::Test;

namespace
{
    const FString PickupItemBPName = TEXT("/Script/Engine.Blueprint'/Game/Inventory/BP_TPSBasePickup.BP_TPSBasePickup'");
    const FString PickupItemBPTestName = TEXT("/Script/Engine.Blueprint'/Game/Test/BP_Test_TPSBasePickup.BP_Test_TPSBasePickup'");
}

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSBasePickup::StaticClass()->GetName());
    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::MatchType::Exact);

    LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {1000.0f}};
    const ATPSBasePickup* Pickup = World->SpawnActor<ATPSBasePickup>(ATPSBasePickup::StaticClass(), InitialTransform);
    if (!TestNull("Pickup item doesn't exist", Pickup))
        return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {1000.0f}};
    const ATPSBasePickup* Pickup = CreateBlueprint<ATPSBasePickup>(World, PickupItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", Pickup))
        return false;

    const auto CollisionComp = Pickup->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("CollisionComp item exists", Pickup))
        return false;

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents() == true);
    TestTrueExpr(Pickup->GetRootComponent() == CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRenderComp = Pickup->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("TextRenderComp item exists", TextRenderComp))
        return false;

    const auto StaticMeshComp = Pickup->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMeshComp item exists", StaticMeshComp))
        return false;

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FPickupDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {2000.0f}};
    ATPSBasePickup* Pickup = CreateBlueprint<ATPSBasePickup>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", Pickup))
        return false;

    FPickupData PickupData {EPickupType::EPT_SPHERE, 120};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(Pickup, "SetPickupData",
                             {
                                 PickupData.ToString(),
                                 Color.ToString(),
                             });

    const auto TextRenderComp = Pickup->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("TextRenderComp item exists", TextRenderComp))
        return false;

    TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(PickupData.Score)));
    TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = Pickup->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMeshComp item exists", StaticMeshComp))
        return false;

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exists", Material))
        return false;

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo {"BaseColor"}, MaterialColor);
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FPickupCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {3000.0f}};
    ATPSBasePickup* Pickup = CreateBlueprint<ATPSBasePickup>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", Pickup))
        return false;

    FPickupData PickupData {EPickupType::EPT_SPHERE, 10};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(Pickup, "SetPickupData",
                             {
                                 PickupData.ToString(),
                                 Color.ToString(),
                             });

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World->GetWorld(), ATPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1))
        return false;

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull("TPSCharacter exists", Character))
        return false;

    const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
    if (!TestNotNull("TPSInvComp exists", InvComp))
        return false;

    const auto InitialPickupScore = InvComp->GetInventoryAmountByType(PickupData.Type);
    TestTrueExpr(InvComp->GetInventoryAmountByType(PickupData.Type) == InitialPickupScore);

    /** Character taken pickup */
    Character->SetActorLocation(InitialTransform.GetLocation());
    TestTrueExpr(InvComp->GetInventoryAmountByType(PickupData.Type) == InitialPickupScore + PickupData.Score);
    TestTrueExpr(!IsValid(Pickup));

    TArray<AActor*> Pickups;
    UGameplayStatics::GetAllActorsOfClass(World->GetWorld(), ATPSBasePickup::StaticClass(), Pickups);
    TestTrueExpr(Pickups.Num() == 0);

    return true;
}

bool FEveryPickupMeshExist::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Test/EmptyTestLevel");

    /*AutomationOpenMap("/Game/Test/EmptyTestLevel");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(360.0f));*/

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    ENUM_LOOP_START(EPickupType, EElem)
    const FTransform InitialTransform = FTransform {FVector {100.0f * (Index + 1)}};
    ATPSBasePickup* Pickup = CreateBlueprint<ATPSBasePickup>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", Pickup))
        return false;

    const FPickupData PickupData {EElem, 10};
    const FLinearColor Color = FLinearColor::Red;
    CallFuncByNameWithParams(Pickup, "SetPickupData",
                             {
                                 PickupData.ToString(),
                                 Color.ToString(),
                             });

    const auto StaticMeshComp = Pickup->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMeshComp item exists", StaticMeshComp))
        return false;

    const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exist"), *UEnum::GetValueAsString(EElem));
    TestNotNull(*MeshMsg, StaticMeshComp->GetStaticMesh().Get());
    ENUM_LOOP_END

    return true;
}

#endif
