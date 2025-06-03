// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATPSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    check(EnhancedInputComponent);

    const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (IsValid(Subsystem))
    {
        Subsystem->AddMappingContext(MappingContext, 0);
    }

    if (IsValid(EnhancedInputComponent))
    {
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATPSPlayerController::ToggleGamePause);
    }
}

void ATPSPlayerController::ToggleGamePause()
{
    bInPause = !bInPause;
    SetPause(bInPause);
    bShowMouseCursor = bInPause;
    bInPause ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false)) : SetInputMode(FInputModeGameOnly());

    OnGamePause.Broadcast(bInPause);
}
