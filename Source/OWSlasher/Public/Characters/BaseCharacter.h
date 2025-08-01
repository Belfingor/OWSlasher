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
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Attack(const FInputActionValue& Value); //For MainCharacter to inherit
	virtual void Attack(); //For Enemy to inherit
	virtual void Die();

	//---------------------------Play Montage Functions----------------------------
	virtual void PlayAttackMontage(bool isMultiAttack = false);
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	//-----------------------------------------------------------------------------
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual bool CanAttack();

	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;
	//-------------------------------Components------------------------------------
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;
	
	//--------------------------------Animation Montages---------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;
	//-----------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

};
