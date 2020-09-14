// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpActor.h"

// Sets default values
ASPowerUpActor::ASPowerUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PowerUpInterval = 0.0f;
	TotalTicks = 0;

}

// Called when the game starts or when spawned
void ASPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
}

//every tick once powerup has been activated
void ASPowerUpActor::OnTickPowerUp()
{
	TicksDone++;

	OnPowerUpTicked();

	if (TicksDone >= TotalTicks)
	{
		OnExpired();

		// Delete timer
		GetWorldTimerManager().ClearTimer(TimerHandlePowerUpTick);
	}
}

void ASPowerUpActor::ActivatePowerUp()
{
	OnActivated();	

	if (PowerUpInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandlePowerUpTick, this, &ASPowerUpActor::OnTickPowerUp, PowerUpInterval, true);
	}
	else 
	{
		OnTickPowerUp();
	}
}

// Called every frame
void ASPowerUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

