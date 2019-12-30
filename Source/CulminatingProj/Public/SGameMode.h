// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 * 
 */
UCLASS()
class CULMINATINGPROJ_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FTimerHandle TimerHandleBotSpawner;

	FTimerHandle TimerHandleNextWaveStart;

	// # of bots to spawn in current wave
	int32 NumBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	// Hook for BP to spawn new bot in map 
	UFUNCTION(BlueprintImplementableEvent, Category ="GameMode")
	void SpawnNextBot();

	void SpawnBotTimerElapsed();

	// Start spawning bots
	void StartWave();

	void CheckWaveState();

	// Stop spawning bots
	void EndWave();

	// Timer to start next wave
	void PrepareForNextWave();

	void CheckIfPlayerAlive();

	void GameOver();

public:

	ASGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
};
