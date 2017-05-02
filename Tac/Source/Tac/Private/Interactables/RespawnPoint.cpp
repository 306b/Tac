// Copyright by GameDream.

#include "Tac.h"
#include "RespawnPoint.h"


// Sets default values
ARespawnPoint::ARespawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpawnRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRange"));
	RootComponent = SpawnRange;
	SpawnRange->bGenerateOverlapEvents = true;
	SpawnRange->OnComponentBeginOverlap.AddDynamic(this, &ARespawnPoint::BeginOccupying);
	//SpawnRange->OnComponentEndOverlap.AddDynamic(this, &ARespawnPoint::EndOccupying);

	bOwnedByA = false;
}

// Called when the game starts or when spawned
void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ARespawnPoint::EndOccupying_Validate() { return true; }

void ARespawnPoint::EndOccupying_Implementation()
{

}

//bool ARespawnPoint::BeginOccupying_Validate(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) { return true; }

void ARespawnPoint::BeginOccupying_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	}
}

void ARespawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(ARespawnPoint, bOwnedByA);
}

bool ARespawnPoint::SetOccupied_Validate(bool bOccupiedByA) { return true; }

void ARespawnPoint::SetOccupied_Implementation(bool bOccupiedByA)
{
	if (bOccupiedByA)
	{
		bOwnedByA = true;
	}
	else
	{
		bOwnedByA = false;
	}
}

bool ARespawnPoint::SpawnPlayer_Validate(AController * PlayerController) { return true; }

void ARespawnPoint::SpawnPlayer_Implementation(AController * PlayerController)
{

}

