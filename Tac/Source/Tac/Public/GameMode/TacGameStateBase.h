// Copyright by GameDream.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "TacGameStateBase.generated.h"

class ATacController;
/**
 * 
 */
UCLASS()
class TAC_API ATacGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
	
public:
	ATacGameStateBase();

	TArray<int32> TeamScores;
	TArray<int32> TeamEnergyAttainRate;

	int32 GetEnergyAttainRate( bool bTeamA);

	TArray<ATacController*> TeamAPlayers;
	TArray<ATacController*> TeamBPlayers;

	virtual void BeginPlay() override;
	
};
