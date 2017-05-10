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
	//UTacWidget();
	UFUNCTION(BlueprintImplementableEvent, Category = "Gear")
	void AddGearSlot(int32 GearIndex);
	// To update RespawnPoint's occupation prograss bar
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void UpdateOccupation(AActor* RP);
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void InitializePlayerState();

};
