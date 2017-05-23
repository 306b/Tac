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
	MoveTime = 0.f;
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
		if (bAtUpper)
		{
			ElevatorFloor->SetWorldLocation(FMath::Lerp<FVector>(Upper, Lower, MoveTime += DeltaTime));
		}
		else
		{
			ElevatorFloor->SetWorldLocation(FMath::Lerp<FVector>(Lower, Upper, MoveTime += DeltaTime));
		}
		if (MoveTime >= 1.f)
		{
			bCanTrigger = true;
			bAtUpper = !bAtUpper;
			MoveTime = 0.f;
		}
	}
}

void ATacElevator::OnTriggered(ATacVehicle* TacPawn)
{
	Super::OnTriggered(TacPawn);

	bCanTrigger = false;
}