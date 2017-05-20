// Copyright by GameDream.

#include "Tac.h"
#include "PickupComponent.h"
#include "Gears.h"
#include "TacHeader.h"
#include "GearManagementComponent.h"
#include "TacVehicle.h"
#include "TacTrigger.h"


// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize owner vehicle, for some reason, the GetOwner() function can only be used once in Constructor
	OwnerVehicle = Cast<ATacVehicle>(GetOwner());

}

// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize here, GetOwner() could be used here
	PickupCapsule = OwnerVehicle->PickupCapsule;
}

// Called every frame
void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPickupComponent::Pickup()
{
	// Try to get gear in range
	TArray<AActor*> ActorsInRange;
	PickupCapsule->GetOverlappingActors(ActorsInRange);
	if (!ActorsInRange.IsValidIndex(0))
	{
		return;
	}
	for (int32 ActorIndex = 0; ActorIndex<ActorsInRange.Num();	)
	{
		auto Gear = Cast<AGears>(ActorsInRange[ActorIndex]);
		if (Gear)
		{
			if (Gear->bPicked)
			{
				ActorsInRange.Remove(Gear);
				continue;
			}
			AGears* GearToAdd = Cast<AGears>(ActorsInRange[ActorIndex]);
			// Try to add the gear to tac
			UGearManagementComponent* Manager = Cast<UGearManagementComponent>(OwnerVehicle->GetGearManager());
			Manager->TryPickup(GearToAdd);
			break;
		}
		auto Trigger = Cast<ATacTrigger>(ActorsInRange[ActorIndex]);
		if (Trigger)
		{
			Trigger->OnPress(OwnerVehicle);
			break;
		}
		ActorIndex++;
	}
}