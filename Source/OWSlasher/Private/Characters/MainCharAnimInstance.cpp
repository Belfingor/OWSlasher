// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); //When overriden "Super" function calls parent function

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		MainCharacterMovementComponent = MainCharacter->GetCharacterMovement();
	}

}
void UMainCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MainCharacterMovementComponent->Velocity);
		isFalling = MainCharacterMovementComponent->IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
	}
}
