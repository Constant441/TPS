// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/TPSGameUserSettings.h"
#include "TPSGameSetting.h"

#define BIND_SETTING_FUNC(FUNC)           \
    Setting->AddSetter([&](int32 Level) { \
        FUNC(Level);                      \
        ApplySettings(false);             \
    });

#define LOCTEXT_NAMESPACE "GameUserSettings"

UTPSGameUserSettings::UTPSGameUserSettings()
{
    const TArray<FSettingOption> VideoOptions = {{LOCTEXT("VFXQualityLow_Loc", "Low"), 0},
                                                 {LOCTEXT("VFXQualityMedium_Loc", "Medium"), 1},
                                                 {LOCTEXT("VFXQualityHigh_Loc", "High"), 2},
                                                 {LOCTEXT("VFXQualityEpic_Loc", "Epic"), 3}};

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetAntiAliasingQuality();
        });
        BIND_SETTING_FUNC(SetAntiAliasingQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Textures_Loc", "Textures"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetTextureQuality();
        });
        BIND_SETTING_FUNC(SetTextureQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetGlobalIlluminationQuality();
        });
        BIND_SETTING_FUNC(SetGlobalIlluminationQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Shadows_Loc", "Shadows"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetShadowQuality();
        });
        BIND_SETTING_FUNC(SetShadowQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PostProcessing_Loc", "Post Processing"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetPostProcessingQuality();
        });
        BIND_SETTING_FUNC(SetPostProcessingQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Reflections_Loc", "Reflections"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetReflectionQuality();
        });
        BIND_SETTING_FUNC(SetReflectionQuality);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Effects_Loc", "Effects"));
        Setting->SetOptions(VideoOptions);
        Setting->AddGetter([&]() {
            return GetVisualEffectQuality();
        });
        BIND_SETTING_FUNC(SetVisualEffectQuality);
        VideoSettings.Add(Setting);
    }
}

UTPSGameUserSettings* UTPSGameUserSettings::Get()
{
    return GEngine ? Cast<UTPSGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UTPSGameSetting*>& UTPSGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void UTPSGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdate.Broadcast();
}

#undef LOCTEXT_NAMESPACE
