#include "Settings/TPSGameSetting.h"
#include "Settings/TPSGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"
#if WITH_AUTOMATION_TESTS

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused,
                                 "TPSGame.UI.PauseMenuShouldBeVisibleOnGamePaused",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/*IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGameUnPaused, "TPSGame.UI.PauseMenuShouldBeCollapsedOnGameUnPaused", EAutomationTestFlags::EditorContext |
 * EAutomationTestFlags::EngineFilter)*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVideoSettingsExist, "TPSGame.UI.AllVideoSettingsExist", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingCanBeApplied, "TPSGame.UI.SettingCanBeApplied", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmark,
                                 "TPSGame.UI.AutoBenchmark",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority)

using namespace TPS::Test;

namespace
{


    void NextSettingClick(int32 SettingIndex)
    {
        const auto VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
        const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
        const auto SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
        const auto NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
        NextSettingButton->OnClicked.Broadcast();
    }

    void DoBenchmarkClick()
    {
        const auto VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
        const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
        BenchmarkButton->OnClicked.Broadcast();
    }
}

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    const auto PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(IsValid(PauseMenuWidget));
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);

    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);

    return true;
}

/*bool FPauseMenuShouldBeCollapsedOnGameUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    const auto PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(IsValid(PauseMenuWidget));
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);
    PausePressed(EnhancedInputComponent);

    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    return true;
}*/

bool FAllVideoSettingsExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    const auto VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(IsValid(VideoSettingsWidget));

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(IsValid(BenchmarkButton));

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(IsValid(VerticalBox));

    const auto& VideoSetting = UTPSGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() > 0);
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSetting.Num());

    return true;
}

bool FSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    const auto GIQualityBefore = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore == VideoSettings[2]->GetCurrentOption().Value);

    NextSettingClick(2);

    const auto GIQualityAfter = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityAfter != GIQualityBefore);
    TestTrueExpr(GIQualityAfter == VideoSettings[2]->GetCurrentOption().Value);

    UTPSGameUserSettings::Get()->SetGlobalIlluminationQuality(GIQualityBefore);
    UTPSGameUserSettings::Get()->ApplySettings(false);

    return true;
}

bool FAutoBenchmark::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    const auto PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(IsValid(PC));

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    PausePressed(EnhancedInputComponent);

    DoBenchmarkClick();

    TArray<int32> SavedSettingValues;
    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings) { SavedSettingValues.Add(Setting->GetCurrentOption().Value); }

    // random clicks

    for (int32 i = 0; i < VideoSettings.Num(); i++) { NextSettingClick(i); }

    for (int32 i = 0; i < VideoSettings.Num(); i++) { TestTrueExpr(SavedSettingValues[i] != VideoSettings[i]->GetCurrentOption().Value); }

    DoBenchmarkClick();

    for (int32 i = 0; i < VideoSettings.Num(); i++) { TestTrueExpr(SavedSettingValues[i] == VideoSettings[i]->GetCurrentOption().Value); }

    return true;
}


#endif
