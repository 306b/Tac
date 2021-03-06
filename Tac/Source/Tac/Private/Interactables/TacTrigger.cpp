// Copyright by GameDream.

#include "Tac.h"
#include "TacTrigger.h"
#include "TacVehicle.h"
#include "TacTriggerables.h"
#include "UnrealNetwork.h"
// Sets default values
ATacTrigger::ATacTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	RootComponent = TriggerMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	CollisionBox->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATacTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATacTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//bool ATacTrigger::OnPress_Validate(ATacVehicle* TacPawn) { return true; }

void ATacTrigger::OnPress(ATacVehicle* TacPawn)
{
	if (Role == ROLE_Authority)
	{
		if (!(TacPawn->UpdateEnergy(-1 * EnergyCost)))
		{
			UE_LOG(LogTemp, Error, TEXT("Energy: %i"), TacPawn->Energy);
			return;
		}
		if (TriggerActor && TriggerActor->bCanTrigger)
		{
			TriggerActor->OnTriggered(TacPawn);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NULL"));
		}
	}
}