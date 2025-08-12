// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class OWSLASHER_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	//-------------------------------Combat----------------------------------------
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack(const FInputActionValue& Value); //For MainCharacter to inherit
	virtual void Attack(); //For Enemy to inherit
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	UFUNCTION(BlueprintCallable) // BlueprintCallable to call it with anim notifies
	virtual void AttackEnd();
	virtual bool CanAttack();
	bool IsAlive();
	//-------------------------------Montage---------------------------------------
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage(bool isMultiAttack = false);
	virtual int32 PlayDeathMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable) // BlueprintCallable to call it with anim notifies
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	//--------------------------------Animation Montages---------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;

	UPROPERTY(BlueprintReadOnly, Category = "Combat") //Just to ensure it does not start uninitialised
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;
private:
	void PlayAnimMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;
};
