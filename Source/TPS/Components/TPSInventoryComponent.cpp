// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSInventoryComponent.h"


UTPSInventoryComponent::UTPSInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UTPSInventoryComponent::TryToAddItem(const FPickupData& PickupData)
{
    if (PickupData.Score < 0)
        return false;

    if (!Inventory.Contains(PickupData.Type))
    {
        Inventory.Add(PickupData.Type, 0);
    }

    const auto NextScore = Inventory[PickupData.Type] + PickupData.Score;
    if (NextScore > InventoryLimits[PickupData.Type])
        return false;

    Inventory[PickupData.Type] = NextScore;

    return true;
}

int32 UTPSInventoryComponent::GetInventoryAmountByType(EPickupType PickupType) const
{
    return Inventory.Contains(PickupType) ? Inventory[PickupType] : 0;
}

void UTPSInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

#if !UE_BUILD_SHIPPING
    const UEnum* PickupEnum = StaticEnum<EPickupType>();
    check(PickupEnum);
    for (int32 i = 0; i < PickupEnum->NumEnums() - 1; i++)
    {
        const EPickupType PickupElem = static_cast<EPickupType>(i);
        const FString PickupElemName = UEnum::GetValueAsString(PickupElem);
        const auto LimitCheckCondition = InventoryLimits.Contains(PickupElem) && InventoryLimits[PickupElem] > 0;
        checkf(LimitCheckCondition, TEXT("Limit for %s doesn't exist or less then zero!"), *PickupElemName);
    }
#endif
}
