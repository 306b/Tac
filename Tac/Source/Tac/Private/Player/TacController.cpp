// Copyright by GameDream.

#include "Tac.h"
#include "TacController.h"
#include "TacSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "TacPlayerState.h"
#include "TacWidget.h"
#include "TacVehicle.h"
#include "TacGameModeBase.h"
#include "RespawnPoint.h"
#include "TacHUD.h"
#include "GearManagementComponent.h"
#include "DamageComponent.h"

ATacController::ATacController()
{
	MyTac = ATacVehicle::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBlueprint> TacPawn(TEXT("Blueprint'/Game/Tac/Core/Characters/BP_Tac.BP_Tac'"));
	if (TacPawn.Succeeded())
	{
		MyTac = (UClass*)TacPawn.Object->GeneratedClass;
	}
}

void ATacController::BeginPlay()
{
	Super::BeginPlay();
	//if (Role < ROLE_Authority)
	//{
	//	FindRPs();
	//}
	FindMonitor();
	FindPlayerStart();
}

// Directly uses input component in controller
void ATacController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Save", IE_Pressed, this, &ATacController::SaveGame);
	//InputComponent->BindAction("Empty", IE_Pressed, this, &ATacController::OnPossessedTacDeath);
}

/*========================================================================================================
	Saving game reference: https://docs.unrealengine.com/latest/INT/Gameplay/SaveGame/Code/index.html
========================================================================================================*/
void ATacController::SaveGame()
{
	/*
	UTacSaveGame* SaveGameInstance = Cast<UTacSaveGame>(UGameplayStatics::CreateSaveGameObject(UTacSaveGame::StaticClass()));
	ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
	// Saves player's name, gears and transform
	SaveGameInstance->PlayerName = TacPS->GetPlayerName();
	SaveGameInstance->Gears = TacPS->GetGears();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	*/

}

void ATacController::LoadGame()
{
	/*
		UTacSaveGame* LoadGameInstance = Cast<UTacSaveGame>(UGameplayStatics::CreateSaveGameObject(UTacSaveGame::StaticClass()));
	ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
	if (!ensure(TacPS)) { return; }
	LoadGameInstance = Cast<UTacSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	if (LoadGameInstance) // Could load from the previous GameInstance
	{
		TacPS->SetPlayerName(LoadGameInstance->PlayerName);
		TacPS->SetGears(LoadGameInstance->Gears);
	}
	else // If there's no GameInstance exists
	{
		TacPS->SetPlayerName(FString(TEXT("NULL")));
		TacPS->EmptyGears();
	}
	*/

}

void ATacController::EmptyGame()
{
	/*
		UTacSaveGame* SaveGameInstance = Cast<UTacSaveGame>(UGameplayStatics::CreateSaveGameObject(UTacSaveGame::StaticClass()));
	ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
	// Reset player respawn location at PlayerStart
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	// Empty player state and save to GameInstance
	TacPS->SetName(TEXT("DEFAULT"));
	TacPS->EmptyGears();
	ATacVehicle* Tac = Cast<ATacVehicle>(GetPawn());
	UGearManagementComponent* Manager = Cast<UGearManagementComponent>(Tac->GetGearManager());
	Manager->ResetGears();
	SaveGameInstance->PlayerName = TacPS->GetPlayerName();
	SaveGameInstance->Gears = TacPS->GetGears();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	*/
}

bool ATacController::SpawnTac_Validate(FTransform SpawnTransform) { return true; }

void ATacController::SpawnTac_Implementation(FTransform SpawnTransform)
{
	if (HasAuthority())
	{
		ATacVehicle* NewTac = GetWorld()->SpawnActorDeferred<ATacVehicle>(MyTac, SpawnTransform, this, Instigator, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);// TODO Spawn BP, teleport warning
		if (NewTac)
		{
			Possess(NewTac);
			UGameplayStatics::FinishSpawningActor(NewTac, NewTac->GetActorTransform());
		}
		auto DamageManager = Cast<UDamageComponent>(NewTac->GetDamageManager());
		DamageManager->OnDeath.AddUniqueDynamic(this, &ATacController::OnPossessedTacDeath);
		UpdateVehicle();
	}
}

bool ATacController::OnPossessedTacDeath_Validate(bool bIsTeamA) { return true; }

void ATacController::OnPossessedTacDeath_Implementation(bool bIsTeamA)// TODO decrease number A in range
{
	if (GetPawn())
	{
		GetPawn()->Destroy();
	}
	UnPossess();
	if (!MonitorCamera) { return; }
	SetViewTargetWithBlend(MonitorCamera, 0.75f);
	HandleHUD(true);
}

void ATacController::HandleHUD_Implementation(bool bEnableInput)
{
	if (IsLocalController())
	{
		if (bEnableInput)
		{
			SetInputMode(FInputModeUIOnly());
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			UE_LOG(LogTemp, Error, TEXT("UI"));
		}
		else
		{
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
			bEnableClickEvents = false;
			UE_LOG(LogTemp, Error, TEXT("Game"));
		}
	}
}

bool ATacController::UpdateVehicle_Validate() { return true; }

void ATacController::UpdateVehicle_Implementation()
{
	ATacVehicle* Tac = Cast<ATacVehicle>(GetPawn());
	Tac->UpdateState();
}

void ATacController::AddGearSlot_Implementation(int32 GearIndex)
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("Is not local controller"));
		return;
	}
	auto TacHud = Cast<ATacHUD>(GetHUD());
	if (TacHud)
	{
		TacHud->AddGearSlot(GearIndex);
	}
}

bool ATacController::ClientPostLogin_Validate()
{
	return true;
}

void ATacController::ClientPostLogin_Implementation()
{
	if (HasAuthority())
	{
		if (!ensure(PlayerState))
		{
			UE_LOG(LogTemp, Error, TEXT("No playerstate"));
			return;
		}
		ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
		TacPS->EmptyGears();
		AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
		if (!ensure(CurrentGameMode))
		{
			UE_LOG(LogTemp, Error, TEXT("No gamemode"));
			return;
		}
		ATacGameModeBase* TacGameMode = Cast<ATacGameModeBase>(CurrentGameMode);
		TacGameMode->RespawnPlayerEvent(this);
		UE_LOG(LogTemp, Error, TEXT("Server"));
	}
}

//void ATacController::FindRPs_Implementation()
//{
//	if (!IsLocalController())
//	{
//		UE_LOG(LogTemp, Error, TEXT("Is not local controller"));
//		return;
//	}
//	TArray<AActor*> FoundActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnPoint::StaticClass(), FoundActors);
//	for (auto Actor : FoundActors)
//	{
//		auto Respawning = Cast<ARespawnPoint>(Actor);
//		Respawnings.Push(Respawning);
//	}
//	auto TacHud = Cast<ATacHUD>(GetHUD());
//	if (TacHud)
//	{
//		TacHud->SetRPs(Respawnings);
//	}
//}

void ATacController::FindMonitor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		if (Actor->ActorHasTag(TEXT("Monitor")))
		{
			auto MC = Cast<ACameraActor>(Actor);
			MonitorCamera = MC;
		}
	}
}

void ATacController::FindPlayerStart()
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

bool ATacController::SpawnRP_Validate(ARespawnPoint* RP) { return true; }

void ATacController::SpawnRP_Implementation(ARespawnPoint* RP)
{
	auto SpawnOrigin = RP->SpawnRange->Bounds.Origin;
	auto SpawnExtent = RP->SpawnRange->Bounds.BoxExtent;
	SpawnOrigin.Z = SpawnOrigin.Z - SpawnExtent.Z + 10.f;
	FTransform Spawns = FTransform(RP->GetActorRotation(), SpawnOrigin, RP->GetActorScale3D());
	SpawnTac(RP->GetActorTransform());
	HandleHUD(false);
}

bool ATacController::InitSpawn_Validate(bool bAsTeamA) { return true; }

void ATacController::InitSpawn_Implementation(bool bAsTeamA)
{
	FTransform SpawnTransform;
	if (bAsTeamA)
	{
		if (!ensure(SpawnStart_A.IsValidIndex(0)))
		{
			UE_LOG(LogTemp, Error, TEXT("No PlayerStart_A for Group_A"));
			return;
		}
		SpawnTransform = SpawnStart_A[0]->GetActorTransform();
		ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
		if (TacPS)
		{
			TacPS->bIsGroup_A = true;
		}
	}
	else
	{
		if (!ensure(SpawnStart_B.IsValidIndex(0)))
		{
			UE_LOG(LogTemp, Error, TEXT("No PlayerStart_B for Group_B"));
			return;
		}
		SpawnTransform = SpawnStart_B[0]->GetActorTransform();
		ATacPlayerState* TacPS = Cast<ATacPlayerState>(PlayerState);
		if (TacPS)
		{
			TacPS->bIsGroup_A = false;
		}
	}
	SpawnTac(SpawnTransform);
	HandleHUD(false);
}

void ATacController::InitCam()
{
	if (HasAuthority())
	{
		SetViewTarget(MonitorCamera);
		HandleHUD(true);
	}
}