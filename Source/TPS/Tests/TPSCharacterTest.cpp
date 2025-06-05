#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#if (WITH_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "Misc/AutomationTest.h"
#include "TPSCharacter.h"
#include "TestUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterTests, All, All);


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage,
                                 "TPSGame.Character.HealthMightBeChangedWithDamage",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait,
                                 "TPSGame.LatentCommand.SimpleWait",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog,
                                 "TPSGame.LatentCommand.SimpleLog",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap,
                                 "TPSGame.LatentCommand.OpenCloseMap",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled,
                                 "TPSGame.Character.CharacterCanBeKilled",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth,
                                 "TPSGame.Character.AutoHealShouldRestoreHealth",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);


using namespace TPS::Test;

namespace
{
    const FString CharacterTestBPName = TEXT("/Script/Engine.Blueprint'/Game/Test/BP_Test_ThirdPersonCharacter.BP_Test_ThirdPersonCharacter'");
}

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Test/EmptyTestLevel");
    // AutomationOpenMap("/Game/Test/EmptyTestLevel");
    // ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(360.0f));

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character))
        return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);


    const float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent {}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPSLatentLogCommand, FString, LogMessage);

bool FTPSLatentLogCommand::Update()
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 1"));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]() {
        UE_LOG(LogTPSCharacterTests, Display, TEXT("Latent log 1"));
        return true;
    }));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 2"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommand("Latent log 2"));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 3"));
    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/EmptyTestLevel");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, ATPSCharacter*, Character, float, LifeSpan);

bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character wasn't destroyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character))
        return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 1.5f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
    Character->FinishSpawning(InitialTransform);

    const auto Controller = World->SpawnActor<APlayerController>(APlayerController::StaticClass());
    if (TestNotNull("PlayerController exists", Controller))
    {
        Controller->Possess(Character);
    }

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    const auto KillingDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KillingDamageAmount, FDamageEvent {}, nullptr, nullptr);
    TestEqual("Health is empty", Character->GetHealthPercent(), 0.0f);

    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == MOVE_None);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EElem) == ECR_Ignore);
    }
    ENUM_LOOP_END

    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(Controller->GetStateName() == NAME_Spectating);
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));


    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(Character, HealthData.LifeSpan));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]() {
            if (IsValid(Character))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character wasn't destroyed"));
            }
        },
        HealthData.LifeSpan));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, ATPSCharacter*, Character, float, HealingDuration);

bool FAutoHealCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Health is not full"));
        }
        return true;
    }
    return false;
}


bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    const FTransform InitialTransform = FTransform {FVector {0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character))
        return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 100.0f;
    HealthData.HealModifier = 5.0f;
    HealthData.HealRate = 0.5f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    const float DamageAmount = 12.5f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent {}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealingDuration = FMath::CeilToInt(HealthData.HealRate * HealthDiff / HealthData.HealModifier);

    // ADD_LATENT_AUTOMATION_COMMAND(FAutoHealCheckLatentCommand(Character, HealingDuration));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]() {
            if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Health is not full"));
            }
        },
        HealingDuration));

    return true;
}

#endif
