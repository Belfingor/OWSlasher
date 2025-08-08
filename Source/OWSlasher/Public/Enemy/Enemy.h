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
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	virtual void Destroyed() override;

private:
	//-------------------------------Components------------------------------------
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

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

	//----------------------------------AI Behaviour-------------------------------
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatroling();
	void ChaseTarget();
	void ClearPatrolTimer();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	//------------------------------------Combat-----------------------------------
	void StartAttackTimer();
	void ClearAttackTimer();

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackTimerMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackTimerMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat) 
	float PatrolingMoveSpeed = 130.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingMoveSpeed = 400.f;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	UFUNCTION() //bound with delegate in PawnSensingComponent, needs UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	UPROPERTY (BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
};
