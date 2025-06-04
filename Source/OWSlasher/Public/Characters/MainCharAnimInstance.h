// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "MainCharAnimInstance.generated.h"

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
	class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* MainCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
	ECharacterState CharacterState;
};
