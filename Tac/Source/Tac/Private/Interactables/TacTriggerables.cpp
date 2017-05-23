// Copyright by GameDream.

#include "Tac.h"
#include "TacTriggerables.h"
#include "TacVehicle.h"

// Sets default values
ATacTriggerables::ATacTriggerables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanTrigger = true;
}

// Called when the game starts or when spawned
void ATacTriggerables::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATacTriggerables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacTriggerables::OnTriggered(ATacVehicle* TacPawn)
{
	
}