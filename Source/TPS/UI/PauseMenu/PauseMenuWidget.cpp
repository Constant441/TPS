// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/PauseMenuWidget.h"

#include "Components/Button.h"
#include "TPSPlayerController.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CloseMenuButton);
    CloseMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseMenuButton);
}

void UPauseMenuWidget::OnCloseMenuButton()
{
    const auto PlayerController = GetOwningPlayer<ATPSPlayerController>();
    if (IsValid(PlayerController))
    {
        PlayerController->ToggleGamePause();
    }
}