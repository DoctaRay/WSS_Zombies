// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "SPowerUpActor.h"
#include "SZombie.h"
#include "STrackerBot.h"


// Sets default values
ASPickupActor::ASPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeLocation(FRotator(90, 0.0f, 0.0f).Vector());
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);
	
	CooldownTime = 10.0f;
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

// Called every frame
void ASPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickupActor::Respawn()
{
	if (PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup %s is nullptr. Update in BP"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//transform should be the same as the pickup actor
	PowerUpInst = GetWorld()->SpawnActor<ASPowerUpActor>(PowerUpClass, GetTransform(), SpawnParams);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ASZombie* isZombie = Cast<ASZombie>(OtherActor);
	ASTrackerBot* isTrackerBot = Cast<ASTrackerBot>(OtherActor);

	//makes sure only player can use powerups 
	if (PowerUpInst && (!isZombie && !isTrackerBot))
	{
		PowerUpInst->ActivatePowerUp();
		PowerUpInst = nullptr;

		// Set time for respawn
		GetWorldTimerManager().SetTimer(TimerHandleRespawnTimer, this, &ASPickupActor::Respawn, CooldownTime);
	}
}

