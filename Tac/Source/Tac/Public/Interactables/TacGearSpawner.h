// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "TacHeader.h"
#include "TacGearSpawner.generated.h"

UCLASS()
class TAC_API ATacGearSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacGearSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gear)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadOnly)
	TArray<FGearSpawn> SpawnStructs;

	UFUNCTION(Category = Spawning, BlueprintCallable)
	void SpawnActors();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
