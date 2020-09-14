// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "SHealthComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "SZombie.h"

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

	// Collision channel to check if DeathBot is close to player
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	ExplosionDamage = 40;
	ExplosionRadius = 200;

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	//OnActorBeginOverlap.AddDynamic(this)
	
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;	
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	// Apply damage to surroundings upon death
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

	//DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Blue, false, 2.0f, 0, 1.0f);


	Destroy();
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
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		//value of parameter in M_TrackerBot BP
		MatInst->SetScalarParameterValue("LastTimeTakenDamage", GetWorld()->TimeSeconds);	
	}

	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}

}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!bStartedSelfDestruction)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
		if (PlayerPawn)
		{
			ASZombie* ZombiePawn = Cast<ASZombie>(OtherActor);
			if (!ZombiePawn)
			{
				//Start self-destruction timer
				GetWorldTimerManager().SetTimer(TimerHandleSelfDamage, this, &ASTrackerBot::SelfDamage, 0.5f, true, 0.0f);

				bStartedSelfDestruction = true;

				UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
			}
		}
	}
	
}

void ASTrackerBot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 50, GetInstigatorController(), this, nullptr);
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


