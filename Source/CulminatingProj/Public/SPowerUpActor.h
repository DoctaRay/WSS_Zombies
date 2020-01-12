// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerUpActor.generated.h"

UCLASS()
class CULMINATINGPROJ_API ASPowerUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTickPowerUp();

	FTimerHandle TimerHandlePowerUpTick;

	//Time between powerups
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float PowerUpInterval;

	//total times we apply powerup
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 TotalTicks;

	int32 TicksDone;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// not BP implementable because powerup should spawn before player spawns
	void ActivatePowerUp();

	//able to implement in BP and call in CPP
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnPowerUpTicked();

};
