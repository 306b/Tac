// Copyright by GameDream.

#include "Tac.h"
#include "Gears/Public/Ejector.h"
#include "Interactables/Public/GearSpawnVolume.h"

// Sets default values
AEjector::AEjector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Ejector = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ejector"));
	RootComponent = Ejector;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> EjectorMesh(TEXT("StaticMesh'/Game/Tac/Art/Gears/SM_Ejector.SM_Ejector'"));
	if (EjectorMesh.Object)
	{
		Ejector->SetStaticMesh(EjectorMesh.Object);
		Ejector->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curvy(TEXT("CurveFloat'/Game/Tac/Gears/Curve_GearsHover_Float.Curve_GearsHover_Float'"));
	if (Curvy.Object)
	{
		TimelineCurve = Curvy.Object;
	}

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FloatTimeline"));
	Timeline->SetTimelineLength(1.5f);
	Timeline->SetLooping(true);

	InterpFunction.BindUFunction(this, FName{ TEXT("EjectorFloat") });

	FloatRange = 5.f;
}

// Called when the game starts or when spawned
void AEjector::BeginPlay()
{
	Super::BeginPlay();

	auto VolumeEjector = Cast<AGearSpawnVolume>(GetOwner());
	Timeline->AddInterpFloat(TimelineCurve, InterpFunction);
	Timeline->PlayFromStart();
}

// Called every frame
void AEjector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Ejector->AddRelativeRotation(FRotator(0.f, 5.f, 0.f));
}

void AEjector::EjectorFloat(float val)
{
	auto CurrentLocation = Ejector->GetComponentLocation();
	auto NewLocationZ =CurrentLocation.Z  + val * FloatRange;
	auto NewLoaction = FVector(CurrentLocation.X, CurrentLocation.Y, NewLocationZ);
	Ejector->SetRelativeLocation(NewLoaction);

}
