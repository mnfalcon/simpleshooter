#pragma once

/**
** THIS CLASS IS OUTSIDE OF UE'S CLASS HIERARCHY
**/

struct FCornerCoordinates
{
	int32 UpperLeftX;
	int32 UpperLeftY;
	int32 LowerRightX;
	int32 LowerRightY;
};

class FloorNode
{
private:
	FCornerCoordinates CornerCoordinates;

	static int32 FloorNodeCount;

public:
	FloorNode();
	FloorNode(const FCornerCoordinates& Coordinates);
	~FloorNode();

	FORCEINLINE FCornerCoordinates GetCornerCoordinates() const { return CornerCoordinates; }
	FORCEINLINE void SetCornerCoordinates(FCornerCoordinates Coordinates) { CornerCoordinates = Coordinates; }
	FORCEINLINE static int32 GetNodeCount() { return FloorNodeCount; }
};