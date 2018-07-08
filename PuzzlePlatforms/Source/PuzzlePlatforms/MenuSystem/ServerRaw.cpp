// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRaw.h"
#include "Components/Button.h"
#include "MenuSystem/MainMenu.h"


void UServerRaw::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RawButton->OnClicked.AddDynamic(this, &UServerRaw::OnClicked);
}

void UServerRaw::OnClicked()
{
	Parent->SelectIndex(Index);
}
