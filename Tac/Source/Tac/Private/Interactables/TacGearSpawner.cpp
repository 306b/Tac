// Copyright by GameDream.

#include "Tac.h"
#include "TacGearSpawner.h"
#include "Gears.h"

// Sets default values
ATacGearSpawner::ATacGearSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = BaseMesh;
	BaseMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ATacGearSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnActors();
}

// Called every frame
void ATacGearSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATacGearSpawner::SpawnActors()
{
	if (HasAuthority())
	{
		auto World = GetWorld();
		if (!ensure(World)) { return; }
		// Uses SpawnParams as SpawnActor()'s reference
		if (!SpawnStructs.IsValidIndex(0)) { return; }
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		int32 Index = FMath::RandRange(0, SpawnStructs.Num() - 1);
		auto SpawnLocation = BaseMesh->GetSocketLocation(TEXT("Gear"));
		auto SpawnRotation = FRotator(0.f, FMath::FRand() * 360.f, 0.f);
		AActor* Spawns = World->SpawnActor<AActor>(SpawnStructs[Index].GetSpawnActor(), SpawnLocation, SpawnRotation, SpawnParams);
		AGears* Gear = Cast<AGears>(Spawns);
		Gear->WorldSpawn();
	}
}
