// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "TacTrigger.generated.h"

class ATacTriggerables;
class ATacVehicle;
UCLASS()
class TAC_API ATacTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacTrigger();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	UStaticMeshComponent* TriggerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Trigger)
	int32 EnergyCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Trigger)
	ATacTriggerables* TriggerActor;

	void OnPress(ATacVehicle* TacPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
