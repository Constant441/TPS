// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SettingOptionWidget.generated.h"

class UTPSGameSetting;
class UTextBlock;
class UButton;

UCLASS()
class TPS_API USettingOptionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingDisplayName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingCurrentValue;

    UPROPERTY(meta = (BindWidget))
    UButton* PrevSettingButton;

    UPROPERTY(meta = (BindWidget))
    UButton* NextSettingButton;

private:
    TWeakObjectPtr<UTPSGameSetting> Setting;

    void Init(UTPSGameSetting* InSetting);
    void UpdateTexts();

    UFUNCTION()
    void OnNextSetting();

    UFUNCTION()
    void OnPrevSetting();

    friend class UVideoSettingsWidget;
};
