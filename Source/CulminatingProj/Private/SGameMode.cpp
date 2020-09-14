// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "SHealthComponent.h"
#include "SPlayerState.h"

ASGameMode::ASGameMode()
{
    TimeBetweenWaves = 5.0f;

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::StartWave()
{
    WaveCount++;

    NumBotsToSpawn = 2 * WaveCount;

    PlayerStateClass = ASPlayerState::StaticClass();

    GetWorldTimerManager().SetTimer(TimerHandleBotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

}

void ASGameMode::CheckWaveState()
{

    bool bIsPreparingNextWave = GetWorldTimerManager().IsTimerActive(TimerHandleNextWaveStart);

    if (NumBotsToSpawn > 0 || bIsPreparingNextWave)
    {
        return;
    }

    bool bIsAnyBotAlive = false;

    for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
    {
        //check if pawn is the player or has not spawned yet
        APawn* TestPawn = It->Get();
        if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
        {
            continue;
       }

       // Check if there are any bots alive
       USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
       if (HealthComp && HealthComp->GetHealth() > 0.0f)
       {
           bIsAnyBotAlive = true;
           break;
       }
    }

    if (!bIsAnyBotAlive)
    {
        PrepareForNextWave();
    }
}

void ASGameMode::EndWave()
{
    UE_LOG(LogTemp, Warning, TEXT("Wave finished"));
    GetWorldTimerManager().ClearTimer(TimerHandleBotSpawner);
    //PrepareForNextWave();
}

void ASGameMode::PrepareForNextWave()
{
	//Sets time between wave. 
    GetWorldTimerManager().SetTimer(TimerHandleNextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::SpawnBotTimerElapsed()
{
    SpawnNextBot();

    NumBotsToSpawn--;

    if (NumBotsToSpawn <= 0)
    {
        EndWave();
    }
}

void ASGameMode::StartPlay()
{
    Super::StartPlay();

    PrepareForNextWave();
}

void ASGameMode::CheckIfPlayerAlive()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            APawn* MyPawn = PC->GetPawn();
            USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
            // Check if player is alive
            if (HealthComp && HealthComp->GetHealth() > 0.0f)
            {
                return;
            }
        }
    }

    GameOver();
}

void ASGameMode::GameOver()
{
    EndWave();

    UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!!"));
}

void ASGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CheckWaveState();

    CheckIfPlayerAlive();
}
