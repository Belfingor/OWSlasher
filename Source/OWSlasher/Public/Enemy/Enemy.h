// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class OWSLASHER_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:

	//-------------------------------Components------------------------------------


	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	//------------------------------Animation Montages-----------------------------


	//-----------------------------------------------------------------------------

	UPROPERTY() //Just to ensure it does not start uninitialised
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	//----------------------------------Navigation---------------------------------
	UPROPERTY() //Always give UPROPERTY to Actor pointers when declaring them.
	class AAIController* EnemyController;

	//Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	const float PatrolingMoveSpeed = 150.f;
	const float ChasingMoveSpeed = 400.f;
	//-----------------------------------------------------------------------------

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	UFUNCTION() //bound with delegate in PawnSensingComponent, needs UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	UPROPERTY (BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
};
