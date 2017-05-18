// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "TacTriggerables.generated.h"

class ATacVehicle;
UCLASS()
class TAC_API ATacTriggerables : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacTriggerables();

	virtual void OnTriggered(ATacVehicle* TacPawn);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Triggerables")
	int32 EnergyCost;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
