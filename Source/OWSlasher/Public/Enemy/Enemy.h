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

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	virtual void Destroyed() override;
	// </AActor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;\
	// </IHitInterface>

protected:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// <ABaseCharacter>
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;
	// </ABaseCharacter>

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
//----------------------------------AI Behaviour-------------------------------
	void InitEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
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
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	UFUNCTION() //bound with delegate in PawnSensingComponent, needs UFUNCTION()
	void PawnSeen(APawn* SeenPawn); //Callback for OnPawnSeen in UPawnSensingComponent
	void SpawnDefaultWeapon();

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY() //Just to ensure it does not start uninitialised
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	
	UPROPERTY() //Always give UPROPERTY to Actor pointers when declaring them.
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget; //Current Patrol Target
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	
	float PatrolWaitMin = 5.f;
	
	float PatrolWaitMax = 10.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrolingMoveSpeed = 130.f;
	
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackTimerMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackTimerMax = 1.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingMoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

};
