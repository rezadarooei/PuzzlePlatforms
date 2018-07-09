// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRaw.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRaw : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	void Setup(class UMainMenu* InParent, uint32 InIndex);

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* RawButton;

	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	class UMainMenu* Parent;

	UPROPERTY()
	uint32 Index;
};
