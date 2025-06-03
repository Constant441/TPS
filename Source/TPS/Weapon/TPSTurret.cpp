// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSTurret.h"

#include "TPSProjectile.h"


ATPSTurret::ATPSTurret()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    check(MeshComponent);
    SetRootComponent(MeshComponent);
}

void ATPSTurret::BeginPlay()
{
    Super::BeginPlay();

    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPSTurret::OnFire, FireFrequency, true, FirstDelay);

    UE_LOG(LogTemp, Display, TEXT("AmmoCount:= %d"), AmmoCount);
    UE_LOG(LogTemp, Display, TEXT("FireFrequency:= %f"), FireFrequency);
}

void ATPSTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    if (GetWorld())
    {
        const FTransform SocketTransform = MeshComponent->GetSocketTransform(MuzzleSocketName);
        auto ProjectileObj = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SocketTransform);
        if (ProjectileObj)
        {
            ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
            ProjectileObj->FinishSpawning(SocketTransform);
        }
    }
}
