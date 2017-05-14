// Copyright by GameDream.

#pragma once

#include "GameFramework/HUD.h"
#include "TacHUD.generated.h"

/**
 * 
 */
class ARespawnPoint;
UCLASS()
class TAC_API ATacHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void SetRPs(TArray<ARespawnPoint*> RPsToSet);
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	TArray<ARespawnPoint*> GetRPs() const;
	UFUNCTION(BlueprintCosmetic, BlueprintImplementableEvent, Category = "Gear")
	void AddGearSlot(int32 GearIndex);
protected:
	TArray<ARespawnPoint*> RPs;

};
