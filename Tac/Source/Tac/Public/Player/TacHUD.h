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
	
	FORCEINLINE 
		UFUNCTION(BlueprintCallable, Category = "Respawn") 
		void SetRP(ARespawnPoint* RPtoSet) { RP = RPtoSet; }
	UFUNCTION(BlueprintCallable, Category = "Respawn")
		ARespawnPoint* GetRP() const;
protected:
	ARespawnPoint* RP;

};
