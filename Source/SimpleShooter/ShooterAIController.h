// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehaviorTree;

	APawn* PlayerPawn;


	//UPROPERTY(EditAnywhere, meta = (Tooltip = "Acceptance Radius for following the player pawn"))
	//	float AcceptanceRadius = 450.f;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	bool IsDead() const;
};
