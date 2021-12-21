// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGunActor::AGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGunActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireRateCounter += 1 * DeltaTime;

	if (FireRateCounter > FireRate)
	{
		FireRateCounter = FireRate + 1;
	}
}

void AGunActor::Shoot()
{
	if (!(FireRateCounter > FireRate))
	{
		return;
	}
	if (Ammo <= 0)
	{
		if (EmptyClipSound)
		{
			UGameplayStatics::SpawnSoundAttached(EmptyClipSound, BaseMesh, TEXT("MuzzleFlashSocket"));
		}
		return;
	}
	if (MuzzleFlash)
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, BaseMesh, TEXT("MuzzleFlashSocket"));
	if (MuzzleSound)
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, BaseMesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bHit = GunTrace(Hit, ShotDirection);
	if (bHit)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 15, FColor::Green, true);
		//UE_LOG(LogTemp, Warning, TEXT("Just shot something at %f %f %f"), EndPoint.X, EndPoint.Y, EndPoint.Z);
		if (HitEffect)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, ShotDirection.Rotation());
		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location);

		AActor* ActorHit = Hit.GetActor();
		if (ActorHit != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();

			Hit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	Ammo--;
	if (Ammo < 0)
	{
		Ammo = 0;
	}
	FireRateCounter = 0.f;
}

bool AGunActor::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Loc;
	FRotator Rot;
	OwnerController->GetPlayerViewPoint(Loc, Rot);
	ShotDirection = -Rot.Vector();

	FVector EndPoint = Loc + Rot.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		Hit, Loc, EndPoint,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params);
}

AController* AGunActor::GetOwnerController() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());

	if (PawnOwner == nullptr)
	{
		return nullptr;
	}

	return PawnOwner->GetController();
}

int32 AGunActor::GetAmmo() const
{
	return Ammo;
}

void AGunActor::IncreaseAmmo(int32 Quantity)
{
	Ammo += Quantity;
}

