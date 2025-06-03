// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "TPSTypes.h"
#include "TPSInventoryComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInventoryComponent();

    bool TryToAddItem(const FPickupData& PickupData);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetInventoryAmountByType(EPickupType PickupType) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TMap<EPickupType, int32> InventoryLimits;

private:
    TMap<EPickupType, int32> Inventory;
};
