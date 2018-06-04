// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"



AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	if (HasAuthority() == true) {
		Super::Tick(DeltaTime);
		FVector Location = GetActorLocation();
		FVector GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
		FVector Direction=(GlobalTargetLocation-Location).GetSafeNormal();

		
		Location+= Direction*Speed*DeltaTime;
		SetActorLocation(Location);
	}
}