// Copyright by GameDream.

#include "Tac.h"
#include "TacPlayerState.h"
#include "Gears.h"
#include "TacGameState.h"
#include "Kismet/GameplayStatics.h"

ATacPlayerState::ATacPlayerState()
{
	bIsGroup_A = false;
	NumKills = 0;
	NumDeaths = 0;
}

void ATacPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATacPlayerState, bIsGroup_A);
	DOREPLIFETIME(ATacPlayerState, TeamNumber);
	DOREPLIFETIME(ATacPlayerState, NumKills);
	DOREPLIFETIME(ATacPlayerState, NumDeaths);
}

TArray<TSubclassOf<AGears>> ATacPlayerState::GetGears()
{
	return Gears;
}

TSubclassOf<AGears> ATacPlayerState::GetGear(int32 GearIndex)
{
	if (Gears.IsValidIndex(GearIndex))
	{
		return Gears[GearIndex];
	}
	else // If has no gears, return null gear
	{
		return AGears::StaticClass();
	}
}

FString ATacPlayerState::GetSocketName(int32 GearIndex)
{
	// Way of getting socket name: https://wiki.unrealengine.com/Enums_For_Both_C%2B%2B_and_BP
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGearSocket"), true);
	if (!EnumPtr) return FString("Invalid");
	int32 EnumNum;
	if (!Gears.IsValidIndex(GearIndex)) { return TEXT("DEFAULT_SOCKET"); }
	switch (Gears[GearIndex].GetDefaultObject()->GearSocket)
	{
	case EGearSocket::ENull:
		EnumNum = 0;
		break;
	case EGearSocket::ELeft:
		EnumNum = 1;
		break;
	case EGearSocket::ERight:
		EnumNum = 2;
		break;
	case EGearSocket::EFront:
		EnumNum = 3;
		break;
	case EGearSocket::EBack:
		EnumNum = 4;
		break;
	default:
		EnumNum = 5;
		break;
	}
	return EnumPtr->GetEnumName(EnumNum);
}

void ATacPlayerState::SetGears(TArray<TSubclassOf<AGears>> GearsToSet)
{
	Gears = GearsToSet;
}

void ATacPlayerState::AddGear_Implementation(int32 GearIndex, TSubclassOf<AGears> GearToAdd)
{
	Gears[GearIndex] = GearToAdd;
	GearsAmount++;
}

FString ATacPlayerState::GetPlayerName()
{
	return MyPlayerName;
}

FName ATacPlayerState::GetGearName(int32 GearIndex)
{
	if (Gears.IsValidIndex(GearIndex))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Access"));
		return Gears[GearIndex].GetDefaultObject()->GearName;
	}
	else // If has no gears, return null gear
	{
		//UE_LOG(LogTemp, Warning, TEXT("No access"));
		return FName("DEFAULT_NAME");
	}
}

void ATacPlayerState::SetName(FString NameToSet)
{
	MyPlayerName = NameToSet;
}

void ATacPlayerState::EmptyGears_Implementation()
{
	Gears.Empty();
	Gears.SetNum(4);
}

void ATacPlayerState::Reset()
{
	Super::Reset();

	//PlayerStates persist across seamless travel.  Keep the same teams as previous match.
	//SetTeamNum(0);
	NumKills = 0;
	NumDeaths = 0;

}

void ATacPlayerState::SetTeam(bool bTeamA)
{
	bIsGroup_A = bTeamA;
	ATacGameState* const MyGameState = GetWorld()->GetGameState<ATacGameState>();
	if (!MyGameState) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("No game state"));
		return; 
	}
	if (MyGameState->TeamScores.Num() == 0)
	{
		MyGameState->TeamScores.AddZeroed(2);
	}
}

int32 ATacPlayerState::GetTeamNum() const
{
	return 1;
}

int32 ATacPlayerState::GetKills() const
{
	return NumKills;
}

int32 ATacPlayerState::GetDeaths() const
{
	return NumDeaths;
}

float ATacPlayerState::GetScore() const
{
	return Score;
}

void ATacPlayerState::ScoreKill(ATacPlayerState* Victim, int32 Points)
{
	NumKills++;
	ScorePoints(Points);
}

void ATacPlayerState::ScoreDeath(ATacPlayerState* KilledBy, int32 Points)
{
	NumDeaths++;
}

void ATacPlayerState::ScorePoints(int32 Points)
{
	ATacGameState* const MyGameState = GetWorld()->GetGameState<ATacGameState>();
	if (MyGameState)
	{
		if (bIsGroup_A)
		{
			MyGameState->TeamScores[0] += Points;
        }
		else
		{
			MyGameState->TeamScores[1] += Points;
		}
	}
	Score += Points;
}