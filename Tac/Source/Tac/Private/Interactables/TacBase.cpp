// Copyright by GameDream.

#include "Tac.h"
#include "TacBase.h"
#include "WidgetComponent.h"

// Sets default values
ATacBase::ATacBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnVolume"));
	CollisionSphere->SetupAttachment(RootComponent);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetIsReplicated(true);

	bReplicates = true;
	Health = 500.f;
}

// Called when the game starts or when spawned
void ATacBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATacBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(ATacBase, Health);
}

float ATacBase::GetHealth()
{
	return Health;
}

float ATacBase::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Health = FMath::Clamp<float>(Health - DamageAmount, 0.f, 500.f);
	if (Health <= 0.f)
	{
		if (bIsTeamA)
		{
			OnWon.Broadcast(false);
		}
		else
		{
			OnWon.Broadcast(true);
		}
	}
	return DamageAmount;
}