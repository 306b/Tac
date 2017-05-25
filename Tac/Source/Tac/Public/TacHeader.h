// Copyright by GameDream.

#pragma once
#include "TacHeader.generated.h"

UENUM(BlueprintType)
enum class EGearSocket : uint8
{
	ENull,
	ELeft,
	ERight,
	EFront,
	EBack
};

UENUM(BlueprintType)
enum class EGearBindKey : uint8
{
	ENull,
	EMouseLook,
	ELClick,
	ERClick,
	EShift,
	ESpace,
	EKeyQ
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	EEngine,
	EBody,
	EWheel
};

USTRUCT()
struct FGearSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WhatToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnRate;

	TSubclassOf<AActor> GetSpawnActor()
	{
		return WhatToSpawn;
	}

	int32 GetSpawnAmount()
	{
		return SpawnAmount;
	}

	float GetSpawnRate()
	{
		return SpawnRate;
	}

	FGearSpawn()
	{
		WhatToSpawn = NULL;
		SpawnAmount = 1;
	}
};
