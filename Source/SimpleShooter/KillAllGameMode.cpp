// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"

void AKillAllGameMode::BeginPlay()
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&AShooterPlayerController::SetPlayerEnabledState,
			true);

		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			InputDelegate, StartDelay, false);
	}
}

void AKillAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerController != nullptr)
	{
		EndGame(false);
	}
	else
	{
		for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
		{
			if (!Controller->IsDead())
			{
				return;
			}
		}
		EndGame(true);
	}
}

void AKillAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerWinner == Controller->IsPlayerController());
	}
}