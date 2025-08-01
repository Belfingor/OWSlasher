// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes")); //No need to attach it as it simply stores data
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Called with value"));
}
void ABaseCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Called without value"));
}

void ABaseCharacter::Die()
{

}

void ABaseCharacter::PlayAttackMontage(bool isMultiAttack)
{

}
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower ImpactPoint to Enemy's Actor location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// GetSafeNormal() normalizes vector for easier calculation as Forward vector is normalized by default
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal(); //Calculating vector from actor location to hit location 

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// As soon as |Forward| = 1 and |ToHit| = 1 -> Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cos (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);

	// Convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// If CrossProduct is pointing down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::AttackEnd()
{

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
