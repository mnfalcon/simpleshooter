// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CirclePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

UBTService_CirclePlayer::UBTService_CirclePlayer()
{
	NodeName = TEXT("CirclePlayerLocationService");
}

void UBTService_CirclePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AIPawn == nullptr)
	{
		return;
	}

	FVector CurrentLocation = AIPawn->GetActorLocation();

	FVector NewLocation = CurrentLocation;
	
	NewLocation.X += FMath::RandRange(-Radius, Radius);
	NewLocation.Y += FMath::RandRange(-Radius, Radius);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);
}