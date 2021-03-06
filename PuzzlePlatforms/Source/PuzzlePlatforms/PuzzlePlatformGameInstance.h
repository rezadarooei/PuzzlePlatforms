// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"
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
		void Host(FString ServerName) override;

		UFUNCTION(Exec)
		void Join(uint32 Index) override;

		
		virtual	void LoadMainMenu() override;

		UFUNCTION()
		virtual void QuitGame() override;

		UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

		UFUNCTION( BlueprintCallable)
		void InGameLoadMenu();

		
		void RefreshServerList() override;

		void StartSession();
		
private:

		TSubclassOf<class UUserWidget> MenuClass;

		TSubclassOf<class UUserWidget> InGameMenuClass;

		class UMainMenu* Menu;

		void OnCreateSessionComplete(FName SessionName, bool Succeed);

		void OnDestroySessionComplete(FName SessionName, bool Succeed);

		void OnFindSessionsComplete(bool bWasSuccessful);

		void OnJoinSessioncomplete(FName SessionNaRme, EOnJoinSessionCompleteResult::Type Result);

		void CreateSession();
		// it doesent need * becaouse it is share pointer

		IOnlineSessionPtr SessionInterface;

		TSharedPtr<class FOnlineSessionSearch> SessionSearch;
		
		FString DesirdServerName;
};
