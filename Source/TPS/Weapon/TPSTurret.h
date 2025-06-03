// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTurret.generated.h"

class ATPSProjectile;

UCLASS(Abstract)
class TPS_API ATPSTurret : public AActor
{
    GENERATED_BODY()

public:
    ATPSTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    int32 AmmoCount {10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret", meta = (Units = s))
    float FireFrequency {1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    FName MuzzleSocketName {"MuzzleSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<ATPSProjectile> ProjectileClass;

private:
    UPROPERTY()
    FTimerHandle FireTimerHandle;

    void OnFire();
};
