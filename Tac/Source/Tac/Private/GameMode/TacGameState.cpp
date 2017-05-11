// Copyright by GameDream.

#include "Tac.h"
#include "TacGameState.h"

ATacGameState::ATacGameState()
{
	bGroupA = false;
}

void ATacGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATacGameState, bGroupA);
	DOREPLIFETIME(ATacGameState, TeamScores);
}

