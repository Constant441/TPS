// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

class UInputMappingContext;
class UInputAction;

UCLASS()
class TPS_API ATPSPlayerController : public APlayerController
{
    GENERATED_BODY()

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* MappingContext;

    /** Pause Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* PauseAction;

public:
    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;

    void ToggleGamePause();

protected:
    virtual void SetupInputComponent() override;

private:
    bool bInPause = false;
};
