#include "FloorNode.h"

int32 FloorNode::FloorNodeCount = 0;

FloorNode::FloorNode()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating FloorNode"));
	++FloorNodeCount;
}

FloorNode::FloorNode(const FCornerCoordinates& Coordinates)
{
	++FloorNodeCount;
	CornerCoordinates.UpperLeftX = Coordinates.UpperLeftX;
	CornerCoordinates.UpperLeftY = Coordinates.UpperLeftY;
	CornerCoordinates.LowerRightX = Coordinates.LowerRightX;
	CornerCoordinates.LowerRightY = Coordinates.LowerRightY;
}

FloorNode::~FloorNode()
{
	--FloorNodeCount;
	UE_LOG(LogTemp, Warning, TEXT("Destroying FloorNode"));
}
