// Copyright by GameDream.

#pragma once

#include "GameFramework/Actor.h"
#include "TacBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameDelegate, bool, bIsAWon);

class UWidgetComponent;
UCLASS()
class TAC_API ATacBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	UWidgetComponent* HealthBar;

	UPROPERTY(BlueprintAssignable)
	FGameDelegate OnWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	bool bIsTeamA;

	UFUNCTION(BlueprintPure, Category = "Base")
	float GetHealth();
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
