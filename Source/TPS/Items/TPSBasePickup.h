// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTypes.h"
#include "TPSBasePickup.generated.h"

class USphereComponent;

UCLASS(Abstract)
class TPS_API ATPSBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ATPSBasePickup();

    static FName CollisionComponentName;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    FPickupData PickupData;
};
