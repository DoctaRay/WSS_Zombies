// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "SHealthComponent.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	OwningHealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	OwningHealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}

/* TODO Deprecated function. Should convert to latest version */
// FVector ASTrackerBot::GetNextPathPoint()
// {
// 	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

// 	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

// 	if(NavPath->PathPoints.Num() > 1)
// 	{
// 		// Return next point in path
// 		return NavPath->PathPoints[1];
// 	}

// 	//Path finding failed
// 	return GetActorLocation();
// }

void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthChanged, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Explode on death

	// @TODO: Pulse the material on hit

	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


