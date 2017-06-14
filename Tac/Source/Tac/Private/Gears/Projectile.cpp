// Copyright by GameDream.

#include "Tac.h"
#include "Projectile.h"
#include "TacVehicle.h"
#include "TacBase.h"
#include "VehicleDamageType.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionSphere;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetNotifyRigidBodyCollision(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bAutoActivate = false;

	DamageAmount = 30.f;
	LaunchSpeed = 10000.f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ATacBase>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageAmount,
			GetInstigatorController(),
			OwnerGun,
			UVehicleDamageType::StaticClass()
		);
	}
	else if (Cast<ATacVehicle>(OtherActor))
	{
		if (OtherActor == Instigator) { return; }// TODO Make projectile ignore owner directly
		if (SweepResult.BoneName.IsValid())
		{
			float DamageFactor = 1.f;
			TArray<TCHAR> HitName = SweepResult.BoneName.ToString().GetCharArray();
			if (FString(4, HitName.GetData()) == FString(TEXT("Wel_")))
			{
				DamageFactor = 0.75f;
			}
			else if (HitName.GetData() == FString(TEXT("Engine")))
			{
				DamageFactor = 2.f;
			}
			UGameplayStatics::ApplyDamage(
				OtherActor,
				DamageAmount * DamageFactor,
				Instigator->GetController(),
				OwnerGun,
				UVehicleDamageType::StaticClass()
			);
		}
	}
	Destroy();
}

/*
bool AProjectile::LaunchProjectile_Validate()
{
	return true;
}
*/

void AProjectile::LaunchProjectile()
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovementComponent->Activate();
}
