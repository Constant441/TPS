#if WITH_AUTOMATION_TESTS

#include "Camera/CameraActor.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect,
                                 "TPSGame.Screenshots.RenderingShouldBeCorrect",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter | EAutomationTestFlags::NonNullRHI)

/*IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHUDShouldBeRendered,
                                 "TPSGame.Screenshots.MainPlayerHUDShouldBeRendered",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter | EAutomationTestFlags::NonNullRHI)*/

/*IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
                                 "TPSGame.Screenshots.HealthWidgetShouldBeRenderedCorrectlyAfterDamage",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter | EAutomationTestFlags::NonNullRHI)*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetBeRenderedWhenGamePaused,
                                 "TPSGame.Screenshots.SettingsWidgetBeRenderedWhenGamePaused",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter | EAutomationTestFlags::NonNullRHI)

using namespace TPS::Test;

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    //(X=2345.000000,Y=1655.000000,Z=320.000000)
    //(Pitch=-10.000000,Yaw=40.000000,Roll=0.000000)
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exist", World))
        return false;

    const FTransform Transform {FRotator(-10.0f, 40.0f, 0.0f), FVector(2345.0f, 1655.0f, 320.0f)};
    auto CameraActor = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("CameraActor exist", CameraActor))
        return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exist", PC))
        return false;

    PC->SetViewTarget(CameraActor);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"))

    return true;
}

/*bool FMainPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("main_player_hud_screenshot"))
    return true;
}*/

/*bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exist", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exist", PC)) return false;

    APawn* Pawn = PC->GetPawn();
    if (!TestNotNull("Pawn exist", Pawn)) return false;

    const float DamageAmount = 40.0f;
    Pawn->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));

    return true;
}*/

bool FSettingsWidgetBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exist", World))
        return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exist", PC))
        return false;

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    if (!TestNotNull("EnhancedInputComponent", EnhancedInputComponent))
        return false;

    PausePressed(EnhancedInputComponent);
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_screenshot"));


    return true;
}

#endif
