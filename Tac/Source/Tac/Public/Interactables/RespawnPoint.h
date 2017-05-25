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
	//UFUNCTION(NetMulticast, Reliable)
	void UpdateOccupation(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetOccupied(bool bOccupiedByA);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateActorsInRange(ATacVehicle* TacPawn, bool bBeginOverlap);

	UFUNCTION()
	void ChangeNumber(bool bIsTeamA);// TODO Delegate with parameter

	UFUNCTION(BlueprintPure, Category = "State")
	float GetOccupationVal();

	UPROPERTY(Replicated)
	bool bShouldOccupy;

	UPROPERTY(Replicated)
	bool bOwnedByA;

	UFUNCTION()
	void UpdateState(bool bIsTeamA);
private:
	//UPROPERTY(Replicated)
	int32 NumAInRange;

	UPROPERTY(Replicated)
	float Val_Occupation;
	
};
