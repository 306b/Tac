// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "RespawnPoint.generated.h"

class ATacVehicle;

UCLASS()
class TAC_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnPoint();

	// Range for spawning players
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning)
	UBoxComponent* SpawnRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn players' function
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlayer(AController* PlayerController);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetOccupied(bool bOccupiedByA);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateActorsInRange(ATacVehicle* TacPawn, bool bBeginOverlap);

	UFUNCTION()
	void ChangeNumber();// TODO Delegate with parameter

private:
	UPROPERTY(Replicated)
	bool bOwnedByA;

	//UPROPERTY(Replicated)
	int32 NumAInRange;

	bool bShouldOccupy;

	float Val_Occupation;
	
};
