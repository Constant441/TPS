// Constant.Inc

#include "EnhancedPlayerInput.h"
#include "Utils/InputRecordingUtils.h"
#include "Utils/JsonUtils.h"
#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Items/TPSBasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump,
                                 "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfToHigh,
                                 "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfToHigh",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsCanBeTakenOnMovement,
                                 "TPSGame.Gameplay.AllItemsCanBeTakenOnMovement",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

/*IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsCanBeTakenOnRecordingMovement, "TPSGame.Gameplay.AllItemsCanBeTakenOnRecordingMovement", EAutomationTestFlags::EditorContext |
 * EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);*/

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded,
                                  "TPSGame.Gameplay.MapsShouldBeLoaded",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);


using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UEnhancedInputComponent*, EnhancedInputComponent);

bool FJumpLatentCommand::Update()
{
    JumpPressed(EnhancedInputComponent);
    return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/InventoryTestLevel1");
    // AutomationOpenMap("/Game/Test/EmptyTestLevel");
    // ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(360.0f));

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character))
        return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exist", InventoryItems.Num(), 1))
        return false;


    auto InputComp = Cast<UEnhancedInputComponent>(Character->InputComponent);
    if (!TestNotNull("InputComp exists", InputComp))
        return false;


    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(InputComp));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, World]() {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f));

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfToHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/InventoryTestLevel2");
    // AutomationOpenMap("/Game/Test/EmptyTestLevel");
    // ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(360.0f));

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character))
        return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exist", InventoryItems.Num(), 1))
        return false;

    auto InputComp = Cast<UEnhancedInputComponent>(Character->InputComponent);
    if (!TestNotNull("InputComp exists", InputComp))
        return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(InputComp));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, World]() {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f));

    return true;
}

bool FAllItemsCanBeTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Test/InventoryTestLevel3");
    // AutomationOpenMap("/Game/Test/EmptyTestLevel");
    // ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(360.0f));

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World))
        return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character))
        return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 9);

    auto InputComp = Cast<UEnhancedInputComponent>(Character->InputComponent);
    if (!TestNotNull("InputComp exists", InputComp))
        return false;

    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(Character->GetController<APlayerController>()->PlayerInput);
    if (!TestNotNull("EnhancedInput exists", EnhancedInput))
        return false;

    const int32 MoveActionIndex = GetActionBindingIndexByName(InputComp, "IA_Move", ETriggerEvent::Triggered);
    TestTrueExpr(MoveActionIndex != INDEX_NONE);
    const UInputAction* InputAction = InputComp->GetActionEventBindings()[MoveActionIndex]->GetAction();

    // Waiting for level load
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Movement forward
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() {
            EnhancedInput->InjectInputForAction(InputAction, FVector2D(0.0f, 1.0f), {}, {});
        },
        []() {}, 2.25f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Jump
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(InputComp));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

    // Movement right
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() {
            EnhancedInput->InjectInputForAction(InputAction, FVector2D(1.0f, 0.0f), {}, {});
        },
        []() {}, 1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Diagonal movement
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() {
            EnhancedInput->InjectInputForAction(InputAction, FVector2D(-1.0f, 1.0f), {}, {});
        },
        []() {}, 1.4f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Movement back
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() {
            EnhancedInput->InjectInputForAction(InputAction, FVector2D(0.0f, -1.0f), {}, {});
        },
        []() {}, 0.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Final check
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, World]() {
        TArray<AActor*> InventoryItems;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
        TestTrueExpr(InventoryItems.Num() == 0);
        return true;
    }));

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(const UWorld* InWorld,
                                   const UEnhancedInputComponent* InInputComponent,
                                   UEnhancedPlayerInput* InPlayerInput,
                                   const TArray<FBindingsData>& InBindingsData) :
        World(InWorld), InputComponent(InInputComponent), PlayerInput(InPlayerInput), BindingsData(InBindingsData)
    {}

    virtual bool Update() override
    {
        if (!IsValid(World) || !IsValid(InputComponent) || !IsValid(PlayerInput))
            return true;

        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->TimeSeconds;
        }

        if (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->GetActionEventBindings().Num(); ++i)
            {
                const auto Action = BindingsData[Index].InputActions[i].Action;
                const auto ActionValue = BindingsData[Index].InputActions[i].Value;
                const auto ActionModifiers = BindingsData[Index].InputActions[i].Action->Modifiers;
                const auto ActionTriggers = BindingsData[Index].InputActions[i].Action->Triggers;

                PlayerInput->InjectInputForAction(Action, ActionValue.ToInputActionValue(), ActionModifiers, ActionTriggers);
            }

            if (++Index >= BindingsData.Num())
                return true;
        }

        return false;
    }

private:
    const UWorld* World;
    const UEnhancedInputComponent* InputComponent;
    UEnhancedPlayerInput* PlayerInput;
    const TArray<FBindingsData> BindingsData;
    int32 Index {0};
    float WorldStartTime {0.0f};
};

/*void FAllItemsCanBeTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> Tests =
    {
        {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap", "CharacterTestInputMainMap.json"},
        {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap", "CharacterTestInputCustomMap.json"},
    };

    for (const auto Test : Tests)
    {
        OutBeautifiedNames.Add(Test.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *Test.MapPath, *Test.JsonName));
    }
}


bool FAllItemsCanBeTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if(!TestTrue("Map name and JSON params should exist", ParsedParams.Num() == 2)) return false;

    const auto Level = LevelScope(ParsedParams[0]);

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World item exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 5);

    const FString FileName = GetTestsDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", !InputData.BindingsData.IsEmpty())) return false;

    APlayerController* PlayerController = Character->GetController<APlayerController>();
    if (!TestNotNull("PlayerController exist", PlayerController)) return false;

    const auto InputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
    if (!TestNotNull("EnhancedInputComponent exist", InputComponent)) return false;

    UEnhancedPlayerInput* PlayerInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
    if (!TestNotNull("EnhancedInput exists", PlayerInput)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, InputComponent, PlayerInput, InputData.BindingsData));

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Final check
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [this, World](){
        TArray<AActor*> InventoryItems;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSBasePickup::StaticClass(), InventoryItems);
        TestTrueExpr(InventoryItems.Num() == 0);
        return true;
        }));

    return true;
}*/

void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    const TArray<TTuple<FString, FString>> TestData = {
        {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap"},
        {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap"},
    };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    return true;
}

#endif
