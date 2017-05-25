// Copyright by GameDream.

#include "Tac.h"
#include "TacVehicle.h"
#include "TacPlayerState.h"
#include "RespawnPoint.h"
#include "DamageComponent.h"
#include "TacWidget.h"
#include "WidgetComponent.h"


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
	SpawnRange->OnComponentBeginOverlap.AddDynamic(this, &ARespawnPoint::OnOverlapBegin);
	SpawnRange->OnComponentEndOverlap.AddDynamic(this, &ARespawnPoint::OnOverlapEnd);

	bOwnedByA = false;
	bShouldOccupy = true;
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
	UpdateOccupation(DeltaTime);
}

void ARespawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARespawnPoint, bOwnedByA);
	DOREPLIFETIME(ARespawnPoint, Val_Occupation);
	DOREPLIFETIME(ARespawnPoint, bShouldOccupy);
	//DOREPLIFETIME(ARespawnPoint, NumAInRange);
}

void ARespawnPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ATacVehicle* TacPawn = Cast<ATacVehicle>(OtherActor);
		if (TacPawn && Cast<USkeletalMeshComponent>(OtherComp))
		{
			auto DamageManager = Cast<UDamageComponent>(TacPawn->GetDamageManager());
			DamageManager->OnDeath.AddUniqueDynamic(this, &ARespawnPoint::UpdateState);
			UpdateActorsInRange(TacPawn, true);
			//UE_LOG(LogTemp, Warning, TEXT("In: Has %i A player"), NumAInRange);
		}
	}
}

void ARespawnPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ATacVehicle* TacPawn = Cast<ATacVehicle>(OtherActor);
		if (TacPawn && Cast<USkeletalMeshComponent>(OtherComp))
		{
			UpdateActorsInRange(TacPawn, false);
			//UE_LOG(LogTemp, Warning, TEXT("Out: Has %i A player"), NumAInRange);
		}
	}
}

//bool ARespawnPoint::UpdateOccupation_Validate(float DeltaTime) { return true; }

void ARespawnPoint::UpdateOccupation(float DeltaTime)
{
	if (Role == ROLE_Authority)
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
			//UE_LOG(LogTemp, Log, TEXT("%f"), Val_Occupation);
		}
	}
}

void ARespawnPoint::ChangeNumber(bool bIsTeamA)
{
	if (bIsTeamA)
	{
		--NumAInRange;
	}
	else
	{
		++NumAInRange;
	}
}

float ARespawnPoint::GetOccupationVal()
{
	return Val_Occupation;
}

bool ARespawnPoint::UpdateActorsInRange_Validate(ATacVehicle* TacPawn, bool bBeginOverlap) { return true; }

void ARespawnPoint::UpdateActorsInRange_Implementation(ATacVehicle* TacPawn, bool bBeginOverlap)
{
	ATacPlayerState* TacPS = Cast<ATacPlayerState>(TacPawn->PlayerState);
	if (TacPS)
	{
		int32 Temp = NumAInRange;
		if (TacPS->bIsGroup_A)
		{
			if (bBeginOverlap)
			{
				NumAInRange = FMath::Clamp<int32>(++NumAInRange, -3, 3);
			}
			else
			{
				NumAInRange = FMath::Clamp<int32>(--NumAInRange, -3, 3);
			}
		}
		else
		{
			if (bBeginOverlap)
			{
				NumAInRange = FMath::Clamp<int32>(--NumAInRange, -3, 3);
			}
			else
			{
				NumAInRange = FMath::Clamp<int32>(++NumAInRange, -3, 3);
			}
		}
		if (NumAInRange != Temp && NumAInRange != 0)
		{
			bShouldOccupy = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PS"));
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

void ARespawnPoint::UpdateState(bool bIsTeamA)
{
	TArray<AActor*> OverlappingActors;
	SpawnRange->GetOverlappingActors(OverlappingActors, ATacVehicle::StaticClass());
	for (auto Actor : OverlappingActors)
	{
		auto TacPawn = Cast<ATacVehicle>(Actor);
		if (!TacPawn) { return; }
		auto TacPS = Cast<ATacPlayerState>(TacPawn->PlayerState);
		if (!TacPS) { return; }
		NumAInRange = 0;
		if (TacPS->bIsGroup_A)
		{
			++NumAInRange;
		}
		else
		{
			--NumAInRange;
		}
	}
}