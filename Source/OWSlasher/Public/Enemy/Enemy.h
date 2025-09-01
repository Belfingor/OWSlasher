// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;

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
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

protected:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// <ABaseCharacter>
	virtual void Die_Implementation() override;
	void SpawnSoul();
	void TrySpawnHealingPotion();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	// </ABaseCharacter>

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
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceRadius = 50.f;
	
	UPROPERTY() //Always give UPROPERTY to Actor pointers when declaring them.
	TObjectPtr<AAIController> EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget; //Current Patrol Target
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	
	float PatrolWaitMin = 5.f;
	
	float PatrolWaitMax = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrolingMoveSpeed = 130.f;
	
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTimerMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTimerMax = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingMoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HealthPotionDropChance = .2f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASoul> SoulClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AHealingPotion> PotionClass;

};
