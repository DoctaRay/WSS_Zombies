// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "BallShooter.generated.h"

/**
 * 
 */
UCLASS()
class CULMINATINGPROJ_API ABallShooter : public ASWeapon
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileClass")
	TSubclassOf<AActor> ProjectileClass;

public:
	virtual void Fire() override;

	

};
