// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolume");
	if (!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;
	TriggerVolume->SetBoxExtent(FVector(50, 50, 100));
	//TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this,&APlatformTrigger::OverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OverlapEnd);
	//TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComp->AttachTo(TriggerVolume);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("It is actvated"))
	for (AMovingPlatform* platform :PlatformsToTriggers)
	{
		platform->AddActiveTriggers();
	}
	
}

void APlatformTrigger::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("It is Deactvated"))
	for (AMovingPlatform* platform : PlatformsToTriggers)
	{
		platform->RemoveActiveTriggers();
	}

}

// void APlatformTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
// {
// 	if (OtherActor) {
// 		UE_LOG(LogTemp,Warning,TEXT("Activated"))
// 	}
// 	
// }
// 
