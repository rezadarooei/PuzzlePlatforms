// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"



bool UMainMenu::Initialize()
{
	bool Sucess=Super::Initialize();
	if (!Sucess) return false;
	if (!ensure(Host != nullptr)) return false;
	Host->OnClicked.AddDynamic(this, &UMainMenu::Cliked);
	return true;
}

void UMainMenu::Cliked()
{
	UE_LOG(LogTemp,Warning,TEXT("Host Cliked"))
}
