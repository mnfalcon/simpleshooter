// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GunActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(GetMesh(), TEXT("Eyes_Position"));
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	PrimaryWeapon = GetWorld()->SpawnActor<AGunActor>(PrimaryWeaponClass);
	SecondaryWeapon = GetWorld()->SpawnActor<AGunActor>(SecondaryWeaponClass);

	/** If adding more weapons, consider using a for loop */

	PrimaryWeapon->SetOwner(this);
	SecondaryWeapon->SetOwner(this);
	SecondaryWeapon->SetActorHiddenInGame(true);

	Weapons.Push(PrimaryWeapon);
	Weapons.Push(SecondaryWeapon);

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None); // hides the default weapon that comes with the mesh
	// weapon_r is the bone of the default weapon on the Wraith mesh

	// at Runtime we cant use SetupAttachment like we do in the constructor

	PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	
	if (!IsPlayerControlled())
	{
		SwitchWeapon(FMath::RandRange(0, Weapons.Num())); // this is so the AI can spawn with a random weapon
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput); // method actually belongs to APawn
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::AddControllerPitchInput); // for gamepad
	PlayerInputComponent->BindAxis(TEXT("MoveSides"), this, &AShooterCharacter::MoveSides);
	PlayerInputComponent->BindAxis(TEXT("LookSides"), this, &AShooterCharacter::AddControllerYawInput); // method actually belongs to APawn
	PlayerInputComponent->BindAxis(TEXT("LookSidesRate"), this, &AShooterCharacter::AddControllerYawInput); // for gamepad
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Released, this, &AShooterCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Shoot"),IE_Pressed, this, &AShooterCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("Sprint"),IE_Pressed, this, &AShooterCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"),IE_Released, this, &AShooterCharacter::StopSprinting);
	PlayerInputComponent->BindAction(TEXT("SwitchWeaponUp"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponUp);
	PlayerInputComponent->BindAction(TEXT("SwitchWeaponDown"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponDown);
	//PlayerInputComponent->BindAxis(TEXT("SwitchWeapon"), this, &AShooterCharacter::SwitchWeapon);
}


void AShooterCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
	/** Unlike the gamepad input, this is a rate. The calculation so that movement does not depend on framerate is already done for us **/
}

void AShooterCharacter::MoveSides(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShooterCharacter::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * RotationRate * GetWorld()->GetDeltaSeconds()); /** This is because the Gamepad does not work the same way as the Mouse
																					It is affected by the frame rate and therefore we need to
																					multiply the Axis Value with DeltaTime (time since last frame)
																					so that the rotation does not vary with the framerate **/
}

void AShooterCharacter::LookSidesRate(float Value)
{
	AddControllerYawInput(Value * RotationRate * GetWorld()->GetDeltaSeconds()); /** This is because the Gamepad does not work the same way as the Mouse
																					It is affected by the frame rate and therefore we need to
																					multiply the Axis Value with DeltaTime (time since last frame)
																					so that the rotation does not vary with the framerate **/
}

void AShooterCharacter::PullTrigger()
{
	if (!bIsSprinting)
	{
		Weapons[CurrentWeapon]->Shoot();
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);

	
	DamageApplied = FMath::Min(CurrentHealth, DamageApplied);
	CurrentHealth -= DamageApplied;

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

	return DamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

float AShooterCharacter::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

void AShooterCharacter::Sprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed * SprintMultiplier;
}

void AShooterCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void AShooterCharacter::SwitchWeapon(float IndexIncrease)
{
	Weapons[CurrentWeapon]->SetActorHiddenInGame(true);
	SetSwitchingWeapon(true);
	CurrentWeapon += IndexIncrease;

	if (CurrentWeapon < 0)
	{
		CurrentWeapon = Weapons.Num() - 1;
	}
	if (CurrentWeapon >= Weapons.Num())
	{
		CurrentWeapon = 0;
	}
	Weapons[CurrentWeapon]->SetActorHiddenInGame(false);
	Weapons[CurrentWeapon]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

	FTimerHandle bWTimerHandle;
	FTimerDelegate bWDelegate = FTimerDelegate::CreateUObject(
		this,
		&AShooterCharacter::SetSwitchingWeapon,
		false);

	GetWorldTimerManager().SetTimer(
		bWTimerHandle,
		bWDelegate, .5f, false);
}

AGunActor* AShooterCharacter::GetCurrentWeapon()
{
	return Weapons[CurrentWeapon];
}

void AShooterCharacter::SetSwitchingWeapon(bool bValue)
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(bValue);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&AShooterPlayerController::SetPlayerEnabledState,
			true);

		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			InputDelegate, SwapWeaponDuration, false);
	}
	bSwitchingWeapon = bValue;
}

void AShooterCharacter::SwitchWeaponUp()
{
	SwitchWeapon(1);
}

void AShooterCharacter::SwitchWeaponDown()
{
	SwitchWeapon(-1);
}