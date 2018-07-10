// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/ServerRaw.h"
#include "Components/TextBlock.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	//Create server row class because we want to use it
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRow_BPClass(TEXT("/Game/MenuSystem/WBP_ServerRaw"));
	if (!ensure(ServerRow_BPClass.Class != nullptr)) return;
	ServerRowClass = ServerRow_BPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Sucess=Super::Initialize();
	if (!Sucess) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
	
	
	return true;
}



void UMainMenu::HostServer()
{
if (MenuInterface!=nullptr)
	{
	MenuInterface->Host();
	}
}

void UMainMenu::SetServerList(TArray<FserverData> ServerNames)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();
	int i = 0;
	for (FserverData& ServerData : ServerNames) 
	{
		Raw = CreateWidget<UServerRaw>(World, ServerRowClass);
		if (!ensure(Raw != nullptr)) return;
		ServerList->AddChild(Raw);
		//Server Name is Utext Block
		Raw->ServerName->SetText(FText::FromString(ServerData.Name));
		Raw->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayer, ServerData.MaxPlayer);
		Raw->ConectionFraction->SetText(FText::FromString(FractionText));

		Raw->Setup(this, i);
		
		i++;
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}
//update each item selected or not

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i) 
	{
		auto Row=Cast<UServerRaw>(ServerList->GetChildAt(i));
		if (Row != nullptr) 
		{
			//true if selectedIndex=current Index
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface!=nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("selected index is %d"), SelectedIndex.GetValue())
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("selected not index i"))
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinGame);
	if (MenuInterface != nullptr) 
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->QuitGame();
	}
}
