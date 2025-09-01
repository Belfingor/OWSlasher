// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "MainCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreassure;
class AHealingPotion;
class UAnimMontage;
class USlashOverlay;

enum class EDodgeDirection : uint8
{
	EDD_None,
	EDD_Forward,
	EDD_Backward,
	EDD_Left,
	EDD_Right
};

UCLASS()
class OWSLASHER_API AMainCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealingPotion* Potion) override;

protected:
	virtual void BeginPlay() override;
	//-------------------------------Input Maping----------------------------------
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CharMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MultiAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FPressAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;
	//-------------------------Callbacks for input--------------------------------
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed(const FInputActionValue& Value);
	void FKeyPressed(const FInputActionValue& Value);
	virtual void Attack(const FInputActionValue& Value) override;
	void MultiAttack(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	//-------------------------Play Montage Functions-----------------------------
	virtual int32 PlayAttackMontage(bool isMultiAttack) override;
	void PlayTwoHandedAttackMontage(bool isMultiAttack);
	void PlayEquipMontage(const FName& SectionName);
	//---------------------------------Combat-------------------------------------
	void EquipWeapon(AWeapon* Weapon, const FName& HandSocketName);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	bool HasEnoughStaminaToDodge();
	bool IsOccupied();
	void Arm();
	void Disarm();
	virtual void Die_Implementation() override;

	UFUNCTION (BlueprintCallable)
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable) 
	void FinishEquipping();
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	bool IsUnoccupied();
	void InitSlashOverlay();
	void SetHUDHealth();
	void SetDodgeDirection(FVector2D Value);
	void RotateCharacterForDodge();
	void RotateCharacterToMatchCamera();
	//--------------------------------States---------------------------------------
	ECharacterState CharacterState = ECharacterState::ECS_Unequiped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	//----------------------------Character Components-----------------------------
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;
	//---------------------------Animation Montages--------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> TwoHandedAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> EquipMontage;
	
	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

	EDodgeDirection DodgeDirection = EDodgeDirection::EDD_None;

public: //Setters and Getters
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
