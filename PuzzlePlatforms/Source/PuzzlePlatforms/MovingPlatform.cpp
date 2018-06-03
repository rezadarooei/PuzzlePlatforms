// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"



AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}
// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	if (HasAuthority() == true) {
		Super::Tick(DeltaTime);
		FVector Location = GetActorLocation();

		Location += FVector(DeltaTime * Speed, 0, 0);
		SetActorLocation(Location);
	}
}