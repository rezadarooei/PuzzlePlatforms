// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

class UStaticMeshComponent;
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
public:
		AMovingPlatform();
		virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,Category="MyCategory")
	float Speed=20;
	UPROPERTY(EditAnywhere, Category = "MyCategory", Meta = (MakeEditWidget = true))
	FVector TargetLocation;
	void AddActiveTriggers();
	void RemoveActiveTriggers();
	
private:
	FVector StartGlobalLocation;
	FVector TargetGlobalLocation;
	UPROPERTY(EditAnywhere)
	int32 ActiveTriggers = 1;

};
