// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSBasePickup.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "TPS/Components/TPSInventoryComponent.h"

FName ATPSBasePickup::CollisionComponentName(TEXT("CollisionComp"));

ATPSBasePickup::ATPSBasePickup()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(ATPSBasePickup::CollisionComponentName);
    SetRootComponent(CollisionComponent);
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(30.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
}

void ATPSBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    if (IsValid(Pawn))
    {
        const auto InvComp = Pawn->FindComponentByClass<UTPSInventoryComponent>();
        if (IsValid(InvComp))
        {
            if (InvComp->TryToAddItem(PickupData))
            {
                Destroy();
            }
        }
    }
}
