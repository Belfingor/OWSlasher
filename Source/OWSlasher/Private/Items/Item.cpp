// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "OWSlasher/DebugMacros.h"



// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("AVG of 1 and 3: %d"), AvgInt);

	float AvgFloat = Avg<float>(3.653f, 4.234f);
	UE_LOG(LogTemp, Warning, TEXT("AVG of 3.653, 4.234: %f"), AvgFloat);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

	DRAW_VECTOR_SINGLEFRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}

