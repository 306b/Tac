// Copyright by GameDream.

#include "Tac.h"
#include "TacGameModeBase.h"
#include "GearSpawnVolume.h"
#include "TacController.h"
#include "TacHUD.h"
#include "TacPlayerState.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ATacGameModeBase::ATacGameModeBase()
{
	/*============================================
		Initialize for the Pawn and Controller
	============================================*/
	/*
	static ConstructorHelpers::FClassFinder<APawn> TacPawnBP(TEXT("/Game/Tac/Core/Characters/BP_Tac"));
	if (!ensure(TacPawnBP.Succeeded())) { return; }
	DefaultPawnClass = TacPawnBP.Class;
	*/
	
	static ConstructorHelpers::FClassFinder<AController> TacController(TEXT("Class'/Script/Tac.TacController'"));
	if (!ensure(TacController.Succeeded())) { return; }
	PlayerControllerClass = TacController.Class;

	HUDClass = ATacHUD::StaticClass();
}

void ATacGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	/*================================
		Spawn all GearSpawnVolumes
	================================*/
	ActiveGearVolume();
	InitSpawnStart();
}

void ATacGameModeBase::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
		ATacPlayerState* NewTacPlayerState = Cast<ATacPlayerState>(NewController->PlayerState);
		NewTacPlayerState->PlayerNumber = GameState->PlayerArray.Num();
		ATacController* NewTacController = Cast<ATacController>(NewController);
		NewTacController->ClientPostLogin();
	}
}

void ATacGameModeBase::RespawnPlayerEvent(AController * PlayerController)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("RespawnPlayerEvent"));
		if (PlayerController->GetPawn())
		{
			PlayerController->GetPawn()->Destroy();
			PlayerController->UnPossess();
		}
		ATacController* TacController = Cast<ATacController>(PlayerController);
		TacController->InitCam();
	}
}

void ATacGameModeBase::ActiveGearVolume()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGearSpawnVolume::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AGearSpawnVolume* SpawnVolumeActor = Cast<AGearSpawnVolume>(Actor);
		if (!ensure(SpawnVolumeActor)) { return; }
		SpawnVolumeActor->SpawnActors();
	}
}

void ATacGameModeBase::InitSpawnStart()
{
	if (!ensure(GetWorld())) { return; }
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		APlayerStart* PlayerStartActor = Cast<APlayerStart>(Actor);
		if (!ensure(PlayerStartActor)) { return; }
		if (PlayerStartActor->PlayerStartTag == TEXT("PlayerA"))
		{
			SpawnStart_A.AddUnique(PlayerStartActor);
		}
		else if (PlayerStartActor->PlayerStartTag == TEXT("PlayerB"))
		{
			SpawnStart_B.AddUnique(PlayerStartActor);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Has Player start B : %i"), SpawnStart_B.Num());
}