// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/VideoSettingsWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/TPSGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    const auto UserSettings = UTPSGameUserSettings::Get();
    if (!IsValid(UserSettings))
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("TPSGameUserSettings is nullptr"));
        return;
    }

    UserSettings->LoadSettings();
    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto VideoSetting : VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClas);
        check(SettingWidget);
        SettingWidget->Init(VideoSetting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton) RunBenchmarkButton->OnClicked.AddDynamic(this, &UVideoSettingsWidget::OnBenchmark);

    UserSettings->OnVideoSettingsUpdate.AddDynamic(this, &UVideoSettingsWidget::OnVideoSettingsUpdate);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto UserSettings = UTPSGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdate()
{
    if (!VideoSettingsContainer)
        return;
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingsOptionWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingsOptionWidget->UpdateTexts();
        }
    }
}