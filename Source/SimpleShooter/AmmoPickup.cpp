// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh)
	{
		StartLocation = Mesh->GetRelativeLocation();
		LocOffsetVector.Z = StartLocation.Z;
		LocOffsetVector.Y = StartLocation.Y;
		LocOffsetVector.X = StartLocation.X;
	}
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEnableRotation)
	{
		FRotator Rotation(0.f);
		Rotation.Yaw = 1.f;
		if (Mesh)
		{
			Mesh->AddLocalRotation(Rotation);
			if (bGoingUp)
			{
				LocOffsetVector.Z += OffsetRate * DeltaTime;
				Mesh->SetRelativeLocation(LocOffsetVector);
				if (LocOffsetVector.Z >= VerticalOffset)
				{
					bGoingUp = false;
				}
			}
			if (!bGoingUp)
			{
				LocOffsetVector.Z -= OffsetRate * DeltaTime;
				Mesh->SetRelativeLocation(LocOffsetVector);
				if (LocOffsetVector.Z <= -VerticalOffset)
				{
					bGoingUp = true;
				}
			}
		}
	}
}

void AAmmoPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

	if (Character != nullptr)
	{
		if (Character->IsPlayerControlled())
		{
			Character->GetCurrentWeapon()->IncreaseAmmo(AmmoContained);
			if (PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
			}
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);

			GetWorldTimerManager().SetTimer(ReactivationTimer, RespawnTime, false);
		}
	}
}

void AAmmoPickup::SetVisibleAndEnableCollision()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}
