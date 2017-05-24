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
	ATacElevator();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Elevator)
	UStaticMeshComponent* ElevatorBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Elevator)
	UStaticMeshComponent* ElevatorFloor;
	virtual void OnTriggered(ATacVehicle* TacPawn) override;
	virtual void Tick(float DeltaTime) override;
private:
	bool bAtUpper;
	FVector Lower;
	FVector Upper;
	float MoveDelta;

	void MoveFloor(float DeltaTime);
protected:
	virtual void BeginPlay() override;
};
