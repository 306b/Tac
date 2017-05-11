// Copyright by GameDream.

#pragma once

#include "GameFramework/GameState.h"
#include "TacGameState.generated.h"

/**
 * 
 */
UCLASS()
class TAC_API ATacGameState : public AGameState
{
	GENERATED_BODY()

public:

	ATacGameState();
	/** number of teams in current game (doesn't deprecate when no players are left in a team) */
	UPROPERTY(Transient, Replicated)
		bool bGroupA;

	/** accumulated score per team */
	UPROPERTY(Transient, Replicated)
		TArray<int32> TeamScores;





	
	
};
