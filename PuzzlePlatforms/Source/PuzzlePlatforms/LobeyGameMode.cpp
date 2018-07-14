// Fill out your copyright notice in the Description page of Project Settings.

#include "LobeyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformGameInstance.h"


void ALobeyGameMode::PostLogin(APlayerController * NewPlayer)
{
	
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobeyGameMode::StartGame, 10);
	}
	
}

void ALobeyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobeyGameMode::StartGame()
{
		auto Gameinstance=Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
		if (Gameinstance == nullptr) { return; }
		Gameinstance->StartSession();
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/Game?listen");
	
}
