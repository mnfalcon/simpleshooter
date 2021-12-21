// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

private:
	void EndGame(bool bIsPlayerWinner);

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PawnKilled(APawn* PawnKilled) override;

	UPROPERTY(EditAnywhere)
		float StartDelay = 4.f;
};
