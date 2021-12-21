#include "Floor.h"
#include "FloorNode.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

Floor::Floor()
{
	FloorGridSizeX = 2500;
	FloorGridSizeY = 2500;
	RoomMinX = 1;
	RoomMinY = 1;
}

Floor::~Floor()
{
}

Floor::Floor(int32 GridSizeX, int32 GridSizeY, int32 MinX, int32 MinY, float Units, float Thickness)
{
	FloorGridSizeX = GridSizeX;
	FloorGridSizeY = GridSizeY;
	RoomMinX = MinX;
	RoomMinY = MinY;
	UnitLength = Units;
	LineThickness = Thickness;
}

void Floor::Partition()
{
	FCornerCoordinates CornerCoordinatesA = { 0, 0, FloorGridSizeX, FloorGridSizeY };
	FloorNodeStack.Push(TSharedPtr<FloorNode>(new FloorNode(CornerCoordinatesA)));


	while (FloorNodeStack.Num() > 0)
	{
		TSharedPtr<FloorNode> A = FloorNodeStack.Pop();

		bool bNodeWasSplit = AttemptSplit(A);

		if (!bNodeWasSplit)
		{
			PartitionedFloor.Push(A);
		}
	}
}

int32 Floor::CoinFlip()
{
	return FMath::RandRange(0, 1);
}

bool Floor::ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation)
{
	FCornerCoordinates CornerCoordinates = InNode->GetCornerCoordinates();

	if (Orientation == ESplitOrientation::ESO_Horizontal)
	{
		int32 FloorLength = CornerCoordinates.LowerRightY - CornerCoordinates.UpperLeftY;

		float SplitChance = (float)FloorLength / (float)FloorGridSizeY;
		SplitChance *= SplitChanceMultiplier;

		float DiceRoll = FMath::FRand();

		if (DiceRoll > SplitChance)
		{
			return false;
		}

		return FloorLength >= (2 * RoomMinY);
	}
	else if (Orientation == ESplitOrientation::ESO_Vertical)
	{
		int32 FloorWidth = CornerCoordinates.LowerRightX - CornerCoordinates.UpperLeftX;

		float SplitChance = (float)FloorWidth / (float)FloorGridSizeX;
		SplitChance *= SplitChanceMultiplier;

		float DiceRoll = FMath::FRand();

		if (DiceRoll > SplitChance)
		{
			return false;
		}

		return FloorWidth >= (2 * RoomMinX);
	}

	return false;
}

bool Floor::AttemptSplit(TSharedPtr<FloorNode> InNode)
{
	int32 HorizontalOrVertical = CoinFlip();

	if (HorizontalOrVertical == 0)
	{
		// try split horizontal
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);
			return true;
		}
		// if it horizontal fails, try vertical
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);
			return true;
		}
	}
	else
	{
		// try split vertical
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);
			return true;
		}
		// if it vertical fails, try horizontal
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);
			return true;
		}
	}

	return false;
}

void Floor::SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	int32 SplitPointY = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftY + RoomMinY, InA->GetCornerCoordinates().LowerRightY - RoomMinY);

	FCornerCoordinates CornerCoordinatesB;
	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightY = SplitPointY;
	CornerCoordinatesB.LowerRightX = InA->GetCornerCoordinates().LowerRightX;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;
	CornerCoordinatesC.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesC.UpperLeftY = SplitPointY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);
}

void Floor::SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	int32 SplitPointX = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftX + RoomMinX, InA->GetCornerCoordinates().LowerRightX - RoomMinX);

	FCornerCoordinates CornerCoordinatesB;
	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightY = InA->GetCornerCoordinates().LowerRightY;
	CornerCoordinatesB.LowerRightX = SplitPointX;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;
	CornerCoordinatesC.UpperLeftX = SplitPointX;
	CornerCoordinatesC.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);
}

void Floor::DrawFloorNodes(UWorld* World, TSubclassOf<AActor> ActorToSpawn)
{
	for (int32 i = 0; i < PartitionedFloor.Num(); i++)
	{
		FCornerCoordinates Coordinates = PartitionedFloor[i]->GetCornerCoordinates();
		DrawFloorNode(World, Coordinates, ActorToSpawn);
		
	}
}

void Floor::DrawFloorNode(UWorld* World, FCornerCoordinates Coordinates, TSubclassOf<AActor> ActorToSpawn)
{
	const FVector UpperLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.UpperLeftY * UnitLength, 1.f);
	const FVector UpperRight(Coordinates.LowerRightX * UnitLength, Coordinates.UpperLeftY * UnitLength, 1.f);
	const FVector LowerLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.LowerRightY * UnitLength, 1.f);
	const FVector LowerRight(Coordinates.LowerRightX * UnitLength, Coordinates.LowerRightY * UnitLength, 1.f);

	FVector RandomPointInSquare = GetRandomPointInSquare(UpperLeft, LowerRight);

	World->SpawnActor<AActor>(ActorToSpawn, RandomPointInSquare, FRotator(0.f));
	DrawDebugCircle(World, RandomPointInSquare, 250.f, 48, FColor::Orange, true, -1.f, 0, 2.f, FVector(0.f, 1.f, 0.f), FVector(1.f, 0.f, 0.f), true);

	DrawDebugLine(World, UpperLeft, UpperRight, FColor::Green, true, -1.f, 0U, LineThickness);
	DrawDebugLine(World, UpperLeft, LowerLeft, FColor::Green, true, -1.f, 0U, LineThickness);
	DrawDebugLine(World, LowerLeft, LowerRight, FColor::Green, true, -1.f, 0U, LineThickness);
	DrawDebugLine(World, UpperRight, LowerRight, FColor::Green, true, -1.f, 0U, LineThickness);

}

FVector Floor::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
	float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
	float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

	return FVector(RandomX, RandomY, 1.f);
}
