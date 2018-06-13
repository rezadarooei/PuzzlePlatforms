// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


bool UInGameMenu::Initialize()
{
	bool Sucess = Super::Initialize();
	if (!Sucess) return false;

 	if (!ensure(CancelInGameButton != nullptr)) return false;
 	CancelInGameButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
 
 	if (!ensure(QuitInGameButton != nullptr)) return false;
 	QuitInGameButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
	return true;
}


 void UInGameMenu::QuitPressed()
 {
	 if (MenuInterface) {
		 TearDown();
		 MenuInterface->LoadMainMenu();
	 }

 }
 
 void UInGameMenu::CancelPressed()
 {
	 TearDown();
 }

