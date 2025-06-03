#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerController.GameCanBePaused", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerController.GameCanBeUnPaused", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


using namespace TPS::Test;


bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);

    TestTrueExpr(PC->IsPaused());
    TestTrueExpr(PC->bShowMouseCursor);


    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);
    PausePressed(EnhancedInputComponent);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    return true;
}


#endif
