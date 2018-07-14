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
const static FName SERVER_NAME_SETTING_KEY = TEXT("ServerName");
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
	UE_LOG(LogTemp,Warning,TEXT("it is sub system %s"),*SubSystem->GetSubsystemName().ToString())
	if (SubSystem)
	{
		 SessionInterface = SubSystem->GetSessionInterface();
		
		if (SessionInterface.IsValid())
		{
			//it tells when it creates fully
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
		
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessioncomplete);
			
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

void UPuzzlePlatformGameInstance::Host(FString ServerName)
{
	DesirdServerName = ServerName;
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





void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeed)
{
	if (!Succeed) 
	{
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
	World->ServerTravel("/Game/Maps/Lobey?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeed)
{
	if (Succeed)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	//new keyword create session to heap not on stack.Make Shareable COnvert c++ Pointer to reference counted pointer
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Sesiion Start"))
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
		//Session Search is for finding the session. && Menu!=nullptr using this becouse we want T array to see on serverlist

		if (bWasSuccessful && SessionSearch.IsValid() && Menu!=nullptr) 
		{
			
			TArray<FserverData> ServerNames;
			
				//using & to reference when you want to work with original items and will not modify them.
				//Session Search is construct.and contains some parameter such as search result and it is Tarray
				for (const FOnlineSessionSearchResult& SearchResults : SessionSearch->SearchResults) 
				{
					FserverData Data;

									
					Data.MaxPlayer = SearchResults.Session.SessionSettings.NumPublicConnections;
					Data.CurrentPlayer = Data.MaxPlayer-SearchResults.Session.NumOpenPublicConnections;
					Data.HostUserName = SearchResults.Session.OwningUserName;
					
					FString ServerName;
					bool BGetSessionSetting=SearchResults.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName);
					if (BGetSessionSetting)
					{
						Data.Name = ServerName;
						UE_LOG(LogTemp, Warning, TEXT("Server Name is:%s"),*ServerName)
					}
					else
					{

						Data.Name = "Cold Not Get Server Name";
					}
					ServerNames.Add(Data);

				}
			Menu->SetServerList(ServerNames);
		}
}


//Create Session with this setting
void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		//setting of session creation
		FOnlineSessionSettings SessionSetting;
		IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
		FString SubSystemName = SubSystem->GetSubsystemName().ToString();
		if (SubSystemName == "NULL") 
		{
			SessionSetting.bIsLANMatch = true;
		}
		else
		{
			SessionSetting.bIsLANMatch = false;
		}

		SessionSetting.NumPublicConnections = 5;
		//shows it is not private
		SessionSetting.bShouldAdvertise = true;

		SessionSetting.bUsesPresence = true;
		SessionSetting.Set(SERVER_NAME_SETTING_KEY, DesirdServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSetting);
		
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) { return; };
	if (!SessionSearch.IsValid()) { return; };
	if (Menu != nullptr)
	{
		Menu->TearDown();

	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformGameInstance::OnJoinSessioncomplete(FName SessionNaRme, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) { return; }

	FString Adress;
	if (!SessionInterface->GetResolvedConnectString(SessionNaRme, Adress))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get Connect string"))
			return;
	};

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Adress is: %s"), *Adress));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
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
