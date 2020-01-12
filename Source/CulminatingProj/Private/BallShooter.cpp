// Fill out your copyright notice in the Description page of Project Settings.


#include "BallShooter.h"

void ABallShooter::Fire() 
{
	if (AmmoCount == 0) {
		return;
	}
	AActor* MyOwner = GetOwner();
	if (MyOwner && ProjectileClass) {
		FVector EyeLocation;
		FRotator EyeRotation;

		UE_LOG(LogTemp, Warning, TEXT("MyOwner(): %s"), *(MyOwner->GetName()));
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

        FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);

        AmmoCount--;
		OnAmmoUsed.Broadcast(AmmoCount);
    }
}
