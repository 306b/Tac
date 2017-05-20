// Copyright by GameDream.

#pragma once

#include "Interactables/TacTriggerables.h"
#include "TacElevator.generated.h"

/**
 * 
 */
UCLASS()
class TAC_API ATacElevator : public ATacTriggerables
{
	GENERATED_BODY()
	
public:
	virtual void OnTriggered(ATacVehicle* TacPawn) override;
	
	
};
