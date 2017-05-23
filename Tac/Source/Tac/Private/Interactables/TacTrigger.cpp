// Copyright by GameDream.

#include "Tac.h"
#include "TacTrigger.h"
#include "TacVehicle.h"
#include "TacTriggerables.h"
// Sets default values
ATacTrigger::ATacTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	RootComponent = TriggerMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	CollisionBox->SetupAttachment(RootComponent);
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

void ATacTrigger::OnPress(ATacVehicle* TacPawn)
{
	if (!(TacPawn->UpdateEnergy(-1 * EnergyCost))) { return; }
	if (TriggerActor && TriggerActor->bCanTrigger)
	{
		TriggerActor->OnTriggered(TacPawn);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NULL"));
	}
}