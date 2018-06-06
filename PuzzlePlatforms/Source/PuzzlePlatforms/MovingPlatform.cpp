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
	StartGlobalLocation = GetActorLocation();

	TargetGlobalLocation = GetTransform().TransformPosition(TargetLocation);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	if(ActiveTriggers>0){
		if (HasAuthority() == true) {
			Super::Tick(DeltaTime);
			FVector Location = GetActorLocation();
			float JourneyLength = (TargetGlobalLocation - StartGlobalLocation).Size();
			float JourneyTravelled = (Location - StartGlobalLocation).Size();
			if (JourneyTravelled >= JourneyLength)
			{
				FVector Swap = StartGlobalLocation;
				StartGlobalLocation = TargetGlobalLocation;
				TargetGlobalLocation = Swap;
			}
			FVector Direction = (TargetGlobalLocation - StartGlobalLocation).GetSafeNormal();

			Location += Direction*Speed*DeltaTime;
			SetActorLocation(Location);

		} 
	}
}

void AMovingPlatform::AddActiveTriggers()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTriggers()
{
	if (ActiveTriggers > 0) {
		ActiveTriggers--;
	}

}
