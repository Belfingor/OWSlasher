// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;


UENUM(BlueprintType)
enum class EWeaponType : uint8 //This type will be set in BP for each weapon so we can check which one we are working with
{
	EWT_OneHanded UMETA(DisplayName = "OneHandedWeapon"),
	EWT_TwoHanded UMETA(DisplayName = "TwoHandedWeapon")
};

UCLASS()
class OWSLASHER_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbersEffect();
	void DisableSphereCollision();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsActorSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation); //No definition in .cpp; This function implemented in Weapon BLueprint.
private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY (EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType;
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	TArray<AActor*> IgnoreActors;

	EWeaponType GetWeaponType() { return WeaponType; }
};
