// Copyright by GameDream.

#include "Tac.h"
#include "TacGameStateBase.h"

ATacGameStateBase::ATacGameStateBase()
{
	
}

void ATacGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	TeamEnergyAttainRate.AddZeroed(2);
	TeamEnergyAttainRate[0] = 1;
	TeamEnergyAttainRate[1] = 1;
	TeamScores.AddZeroed(2);
	TeamScores[0] = 0;
	TeamScores[1] = 1;
}

int32 ATacGameStateBase::GetEnergyAttainRate(bool bTeamA)
{
	return bTeamA ? TeamEnergyAttainRate[0] : TeamEnergyAttainRate[1];
}