// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformGameInstance.generated.h"
class UUserWidget;
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
		UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer);
	
		virtual void Init();
		UFUNCTION(Exec)
		void Host();
		UFUNCTION(Exec)
		void Join(const FString& Adress);
private:

		TSubclassOf<UUserWidget> MenuClass;

};
