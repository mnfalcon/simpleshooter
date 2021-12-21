// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GunActor.h"
#include "ShooterCharacter.generated.h"

class AGunActor;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	void MoveForward(float Value);
	void MoveSides(float Value);

	void LookUpRate(float Value);
	void LookSidesRate(float Value);

	void Sprint();
	void StopSprinting();

	void SwitchWeapon(float IndexIncrease);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSwitchingWeapon = false;
	float SwapWeaponDuration = .5f;
	void SetSwitchingWeapon(bool bValue);

	void SwitchWeaponUp();
	void SwitchWeaponDown();

	UPROPERTY(EditAnywhere)
		float SprintMultiplier = 2.f;
	UPROPERTY(EditAnywhere)
		float BaseMovementSpeed = 325.f;
	UPROPERTY(VisibleAnywhere)
		bool bIsSprinting = false;

	UPROPERTY(EditAnywhere)
		float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGunActor> PrimaryWeaponClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGunActor> SecondaryWeaponClass;

	UPROPERTY()
		AGunActor* PrimaryWeapon;
	UPROPERTY()
		AGunActor* SecondaryWeapon;

	TArray<AGunActor*> Weapons;

	int CurrentWeapon = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* HeadBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	AShooterCharacter();

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	UFUNCTION(BlueprintPure)
		float GetHealthPercentage() const;

	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	AGunActor* GetCurrentWeapon();

	UBoxComponent* GetHeadBoxComponent() const { return HeadBox; }

};
