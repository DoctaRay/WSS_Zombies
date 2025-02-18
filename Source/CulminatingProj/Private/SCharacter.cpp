// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SWeapon.h"
#include "Components/CapsuleComponent.h"
#include "CulminatingProj.h"
#include "SHealthComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	//Spawn the starter weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PrimaryWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	SecondaryWeapon = GetWorld()->SpawnActor<ASWeapon>(EndWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	
	if (PrimaryWeapon && SecondaryWeapon)
	{
		//attach to weapon socket (CPP Version of previous Blueprint Code)
		PrimaryWeapon->SetOwner(this);
		PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		SecondaryWeapon->SetOwner(this);
		SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		
		SecondaryWeapon->SetActorHiddenInGame(true);
		
		bIsPrimaryWeapon = true;

	}

	HealthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Value)
{
	 AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	 AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginAim()
{
	bWantsToZoom = true;
}

void ASCharacter::EndAim()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	bWantsToFire = true;
}

void ASCharacter::EndFire()
{
	bWantsToFire = false;
}

void ASCharacter::SwitchWeapon()
{	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (bIsPrimaryWeapon)
	{
		if (SecondaryWeapon) 
		{
			//attach to weapon socket (CPP Version of previous Blueprint Code)
			PrimaryWeapon->SetActorHiddenInGame(true);
			SecondaryWeapon->SetActorHiddenInGame(false);
			
			bIsPrimaryWeapon = false;
		}
	}
	else 
	{
		
		if (PrimaryWeapon)
		{
			SecondaryWeapon->SetActorHiddenInGame(true);
			PrimaryWeapon->SetActorHiddenInGame(false);

			bIsPrimaryWeapon = true;
		}

	}
}

void ASCharacter::Reload()
{
	if (bIsPrimaryWeapon)
	{
		PrimaryWeapon->Reload();
	}
	else 
	{
		SecondaryWeapon->Reload();
	}
}

void ASCharacter::OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthChanged, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		// Die
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);

	if (bWantsToFire && bIsPrimaryWeapon)
	{
		PrimaryWeapon->Used();
	}

	if (bWantsToFire && !bIsPrimaryWeapon)
	{
		SecondaryWeapon->Used();
	}


}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookSide", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::EndFire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASCharacter::BeginAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASCharacter::EndAim);

	PlayerInputComponent->BindAction("WeaponSwitch", IE_Released, this, &ASCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ASCharacter::Reload);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

