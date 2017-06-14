// Copyright by GameDream.

#include "Tac.h"
#include "River.h"
#include "TacVehicle.h"
#include "DamageComponent.h"

// Sets default values
ARiver::ARiver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = RiverMesh;
	RiverMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	BoxVolume->SetupAttachment(RootComponent);
	BoxVolume->OnComponentBeginOverlap.AddDynamic(this, &ARiver::OnOverlap);
}

// Called when the game starts or when spawned
void ARiver::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARiver::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<USkeletalMeshComponent>(OtherComp))
	{
		auto TacPawn = Cast<ATacVehicle>(OtherActor);
		if (!TacPawn) { return; }
		auto Damager = Cast<UDamageComponent>(TacPawn->GetDamageManager());
		if (!Damager) { return; }
		Damager->CallDeath();
	}
}