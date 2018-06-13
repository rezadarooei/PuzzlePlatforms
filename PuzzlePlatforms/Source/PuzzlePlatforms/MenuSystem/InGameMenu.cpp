// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"



bool UInGameMenu::Initialize()
{
	bool Sucess = Super::Initialize();
	if (!Sucess) return false;

 	if (!ensure(CancelInGameButton != nullptr)) return false;
 	CancelInGameButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelMenu);
 
 	if (!ensure(QuitInGameButton != nullptr)) return false;
 	QuitInGameButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);
	return true;
}


 void UInGameMenu::QuitGame()
 {
 
 }
 
 void UInGameMenu::CancelMenu()
 {
 
 }

