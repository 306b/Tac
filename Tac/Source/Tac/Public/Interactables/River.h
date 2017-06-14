// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "River.generated.h"

UCLASS()
class TAC_API ARiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARiver();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = River)
	UStaticMeshComponent* RiverMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = River)
	UBoxComponent* BoxVolume;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
