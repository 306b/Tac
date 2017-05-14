// Copyright by GameDream.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TacVehicle.h"
#include "TacController.generated.h"

class ATacGameModeBase;
class ARespawnPoint;
/**
 * 
 */

UCLASS()
class TAC_API ATacController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATacController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** Save the game */
	void SaveGame();
	/** Load the game */
	void LoadGame();
	/** Clear game records */
	void EmptyGame();
	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateVehicle();
	UFUNCTION(Client, Reliable)
	void AddGearSlot(int32 GearIndex);
	UFUNCTION(Server, Reliable, WithValidation)
	void ClientPostLogin();
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnTac(FTransform SpawnTransform);
	UFUNCTION(Server, Reliable, WithValidation)
	void OnPossessedTacDeath(bool bIsTeamA);

	void ChooseRP(int32 RPIndex);

	UFUNCTION(Client, Reliable)
	void FindRPs();
	void FindRPs_Implementation();
	void FindMonitor();

	TArray<ARespawnPoint*> Respawnings;

private:
	
	ACameraActor* MonitorCamera;
};
