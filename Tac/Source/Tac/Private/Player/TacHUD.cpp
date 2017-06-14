// Copyright by GameDream.

#include "Tac.h"
#include "TacHUD.h"
#include "RespawnPoint.h"

TArray<ARespawnPoint*> ATacHUD::GetRPs() const
{
	return RPs;
}

void ATacHUD::SetRPs(TArray<ARespawnPoint*> RPsToSet)
{
	for (auto Actor : RPsToSet)
	{
		RPs.Push(Actor);
	}
}
