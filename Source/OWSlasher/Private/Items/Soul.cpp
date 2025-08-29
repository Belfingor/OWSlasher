// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Soul.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/PickupInterface.h"

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = StartLocation + FVector(0.f, 0.f, -2000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		StartLocation,
		EndLocation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Persistent,
		OutHit,
		true
	);
	DesiredZ = OutHit.ImpactPoint.Z + 100.f;

}

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const double LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(FVector(-DeltaLocation));
	}

}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);
		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();
	}
}
