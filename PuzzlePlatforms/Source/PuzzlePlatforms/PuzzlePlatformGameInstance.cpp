// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "Components/Widget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My session Game");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;
	
	
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_LoadingInGame"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
	
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Platform Trigger:%s"), *MenuClass->GetName())
		//it gets from windows.ini and for this now it is null sub system
	IOnlineSubsystem* SubSystem=IOnlineSubsystem::Get();
	if (SubSystem)
	{
		 SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
			
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ptr not found"));
	}
	
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;
	Menu->SetUp();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Menu != nullptr)) return;
	Menu->SetUp();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host()
{
	if (SessionInterface.IsValid()) {
		auto ExistingSession=SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) 
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
		
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		SessionSearch->bIsLanQuery = true;

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Sesiion Start"))
	}
}



void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeed)
{
	if (!Succeed) {
		UE_LOG(LogTemp, Warning, TEXT("Could Not create session"));
		return;
	}
	if (Menu!= nullptr) 
	{
		Menu->TearDown();
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeed)
{
	if (Succeed)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	
	
		if (bWasSuccessful && SessionSearch.IsValid() && Menu!=nullptr) 
		{
			TArray<FString> ServerNames;
			UE_LOG(LogTemp, Warning, TEXT("It is finish"))
				for (const FOnlineSessionSearchResult& SearchResults : SessionSearch->SearchResults) 
				{
					
					UE_LOG(LogTemp, Warning, TEXT("It is finish %s"), *SearchResults.GetSessionIdStr())
					ServerNames.Add(SearchResults.GetSessionIdStr());
				}
			Menu->SetServerList(ServerNames);
		}
		
	

}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSetting;
		SessionSetting.bIsLANMatch = true;
		SessionSetting.NumPublicConnections = 2;
		SessionSetting.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSetting);
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Adress)
{
	if (Menu != nullptr)
	{
		//Menu->TearDown();
		Menu->SetServerList({ "Reza","REza" });
	}
// 	UEngine* Engine = GetEngine();
// 	if (!ensure(Engine != nullptr)) return;
// 	
// 	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Adress is: %s"),*Adress));
// 	APlayerController* PlayerController = GetFirstLocalPlayerController();
// 	if (!ensure(PlayerController != nullptr)) return;
// 	PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/MenuSystem/ManMenu?listen", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::QuitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	const FString & Command = "quit";
	PlayerController->ConsoleCommand(Command);
}
