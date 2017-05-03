// Copyright by GameDream.

#include "Tac.h"
#include "TacVehicle.h"
#include "TacPlayerState.h"
#include "RespawnPoint.h"


// Sets default values
ARespawnPoint::ARespawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpawnRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRange"));
	RootComponent = SpawnRange;
	SpawnRange->SetMobility(EComponentMobility::Static);
	SpawnRange->bGenerateOverlapEvents = true;
	SpawnRange->bMultiBodyOverlap = false;
	SpawnRange->OnComponentBeginOverlap.AddDynamic(this, &ARespawnPoint::BeginOccupying);
	SpawnRange->OnComponentEndOverlap.AddDynamic(this, &ARespawnPoint::EndOccupying);

	bOwnedByA = false;
	bShouldOccupy = false;
	Val_Occupation = 0.f;
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

	if (HasAuthority())
	{
		if (bShouldOccupy && NumAInRange != 0)
		{
			Val_Occupation = FMath::Clamp<float>(Val_Occupation + DeltaTime * float(NumAInRange), -10.f, 10.f);
			if (Val_Occupation == 10.f)
			{
				bShouldOccupy = false;
				SetOccupied(true);
			}
			else if (Val_Occupation == -10.f)
			{
				bShouldOccupy = false;
				SetOccupied(false);
			}
			UE_LOG(LogTemp, Log, TEXT("%f"), Val_Occupation);
		}
	}
}

void ARespawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(ARespawnPoint, bOwnedByA);
//	DOREPLIFETIME(ARespawnPoint, NumAInRange);
}

void ARespawnPoint::BeginOccupying(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ATacVehicle* TacPawn = Cast<ATacVehicle>(OtherActor);
		if (TacPawn && Cast<USkeletalMeshComponent>(OtherComp))
		{
			bShouldOccupy = true;
			ATacPlayerState* TacPS = Cast<ATacPlayerState>(TacPawn->PlayerState);
			if (TacPS->bIsGroup_A)
			{
				NumAInRange = FMath::Clamp<int32>(NumAInRange + 1, -3, 3);
			}
			else
			{
				NumAInRange = FMath::Clamp<int32>(NumAInRange - 1, -3, 3);
			}
			UE_LOG(LogTemp, Warning, TEXT("Has %i A player"), NumAInRange);
		}
	}
}

void ARespawnPoint::EndOccupying(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ATacVehicle* TacPawn = Cast<ATacVehicle>(OtherActor);
		if (TacPawn && Cast<USkeletalMeshComponent>(OtherComp))
		{
			ATacPlayerState* TacPS = Cast<ATacPlayerState>(TacPawn->PlayerState);
			if (TacPS->bIsGroup_A)
			{
				NumAInRange = FMath::Clamp<int32>(NumAInRange - 1, -3, 3);
			}
			else
			{
				NumAInRange = FMath::Clamp<int32>(NumAInRange + 1, -3, 3);
			}
			UE_LOG(LogTemp, Warning, TEXT("Has %i A player"), NumAInRange);
		}
	}
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

