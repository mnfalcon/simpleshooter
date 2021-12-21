// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunActor.generated.h"

class UParticleSystem;

UCLASS()
class SIMPLESHOOTER_API AGunActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
		USoundBase* EmptyClipSound;

	UPROPERTY(EditAnywhere)
		float MaxRange = 20000;

	UPROPERTY(EditAnywhere)
		float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Ammo = 32;

	UPROPERTY(EditDefaultsOnly, meta = (Tooltip = "A rate of 0 disables the limitations"))
		float FireRate = 0.f;

	float FireRateCounter = 0.f;
	bool CanShoot();

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AGunActor();

	void Shoot();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmo() const;
	void IncreaseAmmo(int32 Quantity);

};
