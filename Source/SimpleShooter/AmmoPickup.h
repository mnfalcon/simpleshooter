// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class SIMPLESHOOTER_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		int32 AmmoContained = 16;
	UPROPERTY(EditAnywhere)
		bool bHasWeaponRestriction = false;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
		class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* PickupSound;

	UPROPERTY(EditAnywhere)
		bool bEnableRotation = true;

	UPROPERTY(EditAnywhere)
		float OffsetRate = 10.f;
	UPROPERTY(EditAnywhere)
	float VerticalOffset = 50.f;
	FVector LocOffsetVector;
	FVector StartLocation;
	bool bGoingUp = true;

	UPROPERTY(EditAnywhere)
		float RespawnTime = 30.f;

	FTimerHandle ReactivationTimer;

	void SetVisibleAndEnableCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AAmmoPickup();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

};
