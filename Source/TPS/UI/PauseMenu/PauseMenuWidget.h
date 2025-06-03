// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class TPS_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CloseMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnCloseMenuButton();
};
