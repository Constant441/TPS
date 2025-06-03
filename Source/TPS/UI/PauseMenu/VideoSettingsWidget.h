// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "VideoSettingsWidget.generated.h"

class UButton;
class UVerticalBox;
class USettingOptionWidget;

UCLASS()
class TPS_API UVideoSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VideoSettingsContainer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<USettingOptionWidget> SettingOptionWidgetClas;

    UPROPERTY(meta = (BindWidget))
    UButton* RunBenchmarkButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBenchmark();

    UFUNCTION()
    void OnVideoSettingsUpdate();
};
