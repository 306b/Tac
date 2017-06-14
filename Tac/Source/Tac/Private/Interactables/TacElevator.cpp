// Copyright by GameDream.

#include "Tac.h"
#include "TacElevator.h"
#include "TacVehicle.h"

ATacElevator::ATacElevator()
{
	ElevatorBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = ElevatorBase;
	ElevatorBase->SetMobility(EComponentMobility::Static);
	ElevatorBase->SetIsReplicated(true);

	ElevatorFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	ElevatorFloor->SetupAttachment(RootComponent, TEXT("Lower"));
	ElevatorFloor->SetMobility(EComponentMobility::Movable);
	ElevatorFloor->SetIsReplicated(true);

	bAtUpper = false;
	bReplicates = true;
	MoveDelta = 0.f;
}

void ATacElevator::BeginPlay()
{
	Super::BeginPlay();
	Lower = ElevatorBase->GetSocketLocation(TEXT("Lower"));
	Upper = ElevatorBase->GetSocketLocation(TEXT("Upper"));
}

void ATacElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bCanTrigger)
	{
		MoveFloor(DeltaTime);
	}
}

void ATacElevator::OnTriggered(ATacVehicle* TacPawn)
{
	Super::OnTriggered(TacPawn);
	bCanTrigger = false;
}

//bool ATacElevator::MoveFloor_Validate(float DelatTime) { return true; }

void ATacElevator::MoveFloor(float DeltaTime)
{
	if (bAtUpper)
	{
		ElevatorFloor->SetWorldLocation(FMath::Lerp<FVector>(Upper, Lower, MoveDelta += DeltaTime * 0.2));
	}
	else
	{
		ElevatorFloor->SetWorldLocation(FMath::Lerp<FVector>(Lower, Upper, MoveDelta += DeltaTime * 0.2));
	}
	if (MoveDelta >= 1.f)
	{
		bCanTrigger = true;
		bAtUpper = !bAtUpper;
		MoveDelta = 0.f;
	}
}