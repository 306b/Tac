// Copyright by GameDream.

#include "Tac.h"
#include "TacController.h"
#include "TacSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "TacPlayerState.h"
#include "GearWidget.h"
#include "TacVehicle.h"
#include "TacGameModeBase.h"
#include "RespawnPoint.h"
#include "GearManagementComponent.h"
#include "DamageComponent.h"

ATacController::ATacController()
{
	static ConstructorHelpers::FClassFinder<UGearWidget> Widget(TEXT("/Game/Tac/Core/Characters/WBP_TacView"));
	if (Widget.Succeeded())
	{
		PlayerView = Widget.Class;
	}
	TacView = nullptr;

}

void ATacController::BeginPlay()
{
	Super::BeginPlay();
}

// Directly uses input component in controller
void ATacController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Save", IE_Pressed, this, &ATacController::SaveGame);
	InputComponent->BindAction("Load", IE_Pressed, this, &ATacController::ChooseRP);
	InputComponent->BindAction("Empty", IE_Pressed, this, &ATacController::OnPossessedTacDeath);
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
		ATacVehicle* NewTac = GetWorld()->SpawnActorDeferred<ATacVehicle>(ATacVehicle::StaticClass(), SpawnTransform);// TODO Spawn BP, teleport warning
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

bool ATacController::OnPossessedTacDeath_Validate() { return true; }

void ATacController::OnPossessedTacDeath_Implementation()// TODO decrease number A in range
{
	if (GetPawn())
	{
		GetPawn()->Destroy();
	}
	UnPossess();
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
	TacView->AddGearSlot(GearIndex);
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
		//LoadGame();
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client"));
	}
}

void ATacController::UpdateHUD_Implementation()
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("Is not local controller"));
		return;
	}
	TacView = CreateWidget<UGearWidget>(this, PlayerView);
	if (TacView)
	{
		TacView->AddToViewport();
	}
}

void ATacController::ChooseRP()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnPoint::StaticClass(), FoundActors);
	FTransform SpawnTransform;
	if (FoundActors.Num())
	{
		SpawnTransform = FoundActors[0]->GetActorTransform();
		SpawnTac(SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No RP"));
	}
}
