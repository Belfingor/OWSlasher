// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealingPotion.generated.h"

/**
 * 
 */
UCLASS()
class OWSLASHER_API AHealingPotion : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	float PotionHealth;
	
public:
	FORCEINLINE float GetPotionHealthAmount() { return PotionHealth; }
};
