#pragma once
#include "FloorNode.h"

/**
** THIS CLASS IS OUTSIDE OF UE'S CLASS HIERARCHY
**/

class FloorNode;

enum class ESplitOrientation
{
	// ESO = Enum Split Orientation
	ESO_Horizontal,
	ESO_Vertical
};

class Floor
{

private:
	TArray<TSharedPtr<FloorNode>> FloorNodeStack;
	TArray<TSharedPtr<FloorNode>> PartitionedFloor;

	int32 FloorGridSizeX;
	int32 FloorGridSizeY;
	int32 RoomMinX;
	int32 RoomMinY;

	float UnitLength = 200.f;
	float SplitChanceMultiplier = .5f;

	float LineThickness = 10.f;

public:
	Floor();
	~Floor();

	Floor(int32 GridSizeX, int32 GridSizeY, int32 MinX, int32 MinY, float Units, float Thickness);

	void Partition();
	int32 CoinFlip(); // returns a 0 or 1 randomly

	bool ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation);
	bool AttemptSplit(TSharedPtr<FloorNode> InNode);

	void SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);
	void SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);

	void DrawFloorNodes(class UWorld* World, TSubclassOf<class AActor> ActorToSpawn);
	void DrawFloorNode(UWorld* World, FCornerCoordinates Coordinates, TSubclassOf<AActor> ActorToSpawn);

	FORCEINLINE TArray<TSharedPtr<FloorNode>> GetPartitionedFloor() const { return PartitionedFloor; }

	FVector GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight);

};