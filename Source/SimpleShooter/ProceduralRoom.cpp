// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRoom.h"
#include "DrawDebugHelpers.h"
#include "FloorNode.h"
#include "Floor.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorComponent"));
	FloorMesh->SetupAttachment(RootComponent);

	GridSizeX = 5;
	GridSizeY = 5;
	SquareWidth = 200.f;
	GridHeight = 1.f;

	RoomLength = 1000.f;
	RoomWidth = 1000.f;

	SpawnRadius = 25.f;

	TopLeft = FVector(0.f);
	BottomRight = FVector(1000.f, 1000.f, 0);
}

void AProceduralRoom::SpawnItem(UClass* ItemToSpawn)
{
	/*float XLoc = FMath::FRandRange()
	GetWorld()->SpawnActor<AActor>(ItemToSpawn, )*/
}

// Called when the game starts or when spawned
void AProceduralRoom::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateMap();
}

// Called every frame
void AProceduralRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralRoom::CreateGrid()
{
	for (int32 i = 0; i < GridSizeX +1; i++)
	{
		FVector Start = TopLeft + FVector(i * SquareWidth, 0.f, GridHeight);
		FVector End = Start + FVector(0.f, RoomLength, GridHeight);
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, -1.f, 0U, 1.f);
	}

	for (int32 i = 0; i < GridSizeY + 1; i++)
	{
		FVector Start = TopLeft + FVector(0, i * SquareWidth, GridHeight);
		FVector End = Start + FVector(RoomWidth, 0, GridHeight);
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, -1.f, 0U, 1.f);
	}
}

FVector AProceduralRoom::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
	float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
	float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

	return FVector(RandomX, RandomY, GridHeight);
}

void AProceduralRoom::PlacePointsOnGrid()
{
	for (int32 i = 0; i < GridSizeX; i++)
	{
		for (int32 j = 0; j < GridSizeY; j++)
		{
			FVector UpperLeft(i * SquareWidth + SpawnRadius, j * SquareWidth + SpawnRadius, GridHeight);
			FVector LowerRight(i * SquareWidth + SquareWidth - SpawnRadius, j * SquareWidth + SquareWidth - SpawnRadius, GridHeight);
			FVector RandomPointInSquare = GetRandomPointInSquare(UpperLeft, LowerRight);

			DrawDebugPoint(GetWorld(), RandomPointInSquare, 5.f, FColor::Orange, true);
			DrawDebugCircle(GetWorld(), RandomPointInSquare, SpawnRadius, 48, FColor::Orange, true, -1.f, 0, 2.f, FVector(0.f, 1.f, 0.f), FVector(1.f, 0.f, 0.f), true);
		
			//GetWorld()->SpawnActor<AActor>(ActorToSpawn, RandomPointInSquare, FRotator(0.f));
			
		}
	}
}

void AProceduralRoom::GenerateMap()
{
	//CreateGrid();
	//PlacePointsOnGrid();

	TSharedPtr<Floor> Room(new Floor(FloorGridSizeX, FloorGridSizeY, FloorMinX, FloorMinY, FloorUnitLength, 5.f));
	Room->Partition();
	Room->DrawFloorNodes(GetWorld(), ActorToSpawn);
}
