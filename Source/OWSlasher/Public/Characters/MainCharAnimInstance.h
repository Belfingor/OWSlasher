// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "MainCharAnimInstance.generated.h"


class AMainCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class OWSLASHER_API UMainCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMainCharacter> MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> MainCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | States")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = " Movement | States")
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = " Movement | States")
	TEnumAsByte<EDeathPose> DeathPose;
};
