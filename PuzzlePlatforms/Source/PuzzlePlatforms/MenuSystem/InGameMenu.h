// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
private:
	
 	UPROPERTY(meta=(BindWidget))
 	class UButton* CancelInGameButton;
 
 	UPROPERTY(meta = (BindWidget))
 	class UButton* QuitInGameButton;

protected:

	virtual bool Initialize() override;
 	
 	UFUNCTION()
 	void QuitPressed();
 
 	UFUNCTION()
 	void CancelPressed();

};
