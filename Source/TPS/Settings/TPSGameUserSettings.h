// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TPSGameUserSettings.generated.h"

class UTPSGameSetting;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsUpdateDelegate);

UCLASS()
class TPS_API UTPSGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UTPSGameUserSettings();
    static UTPSGameUserSettings* Get();

    const TArray<UTPSGameSetting*>& GetVideoSettings() const;

    /**
     * Runs hardware benchmark
     * Sets the most optimal video settings for your system
     */
    void RunBenchmark();

    FOnSettingsUpdateDelegate OnVideoSettingsUpdate;

private:
    UPROPERTY()
    TArray<UTPSGameSetting*> VideoSettings;
};
