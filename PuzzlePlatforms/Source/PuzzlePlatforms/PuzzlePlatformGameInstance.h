// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance,public IMenuInterface
{
	GENERATED_BODY()


public:
		UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer);
	
		virtual void Init();

		UFUNCTION(Exec)
		void Host() override;

		UFUNCTION(Exec)
		void Join(const FString& Adress) override;

		
		virtual	void LoadMainMenu() override;

		UFUNCTION()
		virtual void QuitGame() override;

		UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

		UFUNCTION( BlueprintCallable)
		void InGameLoadMenu();

		
		
private:

		TSubclassOf<class UUserWidget> MenuClass;

		TSubclassOf<class UUserWidget> InGameMenuClass;

		class UMainMenu* Menu;

		void OnCreateSessionComplete(FName SessionName, bool Succeed);

		IOnlineSessionPtr SessionInterface;
};
