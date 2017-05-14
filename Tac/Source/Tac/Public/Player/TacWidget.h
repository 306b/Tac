// Copyright by GameDream.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TacWidget.generated.h"

/**
 * 
 */

UCLASS()
class TAC_API UTacWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// To update RespawnPoint's occupation prograss bar
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void UpdateOccupation(float Val);
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void InitializePlayerState();

};
