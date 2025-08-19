// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 420.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VIewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("Head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("Head");

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(FPressAction, ETriggerEvent::Triggered, this, &AMainCharacter::FKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainCharacter::Attack);
		EnhancedInputComponent->BindAction(MultiAttackAction, ETriggerEvent::Triggered, this, &AMainCharacter::MultiAttack);
	}
}

void AMainCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void AMainCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AMainCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AMainCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSoulsTextCount(Attributes->GetSouls());
	}
}

void AMainCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGoldTextCount(Attributes->GetGold());
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharMappingContext, 0);
		}
	}

	Tags.Add(FName("EngageableTarget"));
	InitSlashOverlay();
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return; // Prevent movement while attacking
	const FVector2D MovementVector = Value.Get<FVector2D>();	
	if ((MovementVector.Y != 0.f)|| (MovementVector.X != 0.f) && GetController())
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FVector Forward = GetActorForwardVector();
		AddMovementInput(ForwardDirection, MovementVector.Y);
		FVector Right = GetActorRightVector();
		AddMovementInput(SideDirection, MovementVector.X);
	}
	

}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AMainCharacter::EKeyPressed(const FInputActionValue& Value)
{
	// pick up and item if overlapping it
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	FName HandSocketName ("RightHandSocket");

	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon, HandSocketName);
	}
}

void AMainCharacter::FKeyPressed(const FInputActionValue& Value)
{

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
	{
		if (CanDisarm()) Disarm();
		else if (CanArm()) Arm();

	}
	else if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
	{
		if (CanDisarm()) Disarm();
		else if (CanArm()) Arm();
	}
}

void AMainCharacter::Attack(const FInputActionValue& Value) 
{
	Super::Attack(Value);
	if (CanAttack())
	{
		switch (CharacterState)
		{
		case ECharacterState::ECS_EquippedOneHandedWeapon:
			PlayAttackMontage(false);
			ActionState = EActionState::EAS_Attacking;
			break;
		case ECharacterState::ECS_EquippedTwoHandedWeapon:
			PlayTwoHandedAttackMontage(false);
			ActionState = EActionState::EAS_Attacking;
			break;
		default:
			break;
		}
	}
	//----------------------------------------------------------------------------- temp solution to rotate character when attacking
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	SetActorRotation(YawRotation);
	//-----------------------------------------------------------------------------
}

void AMainCharacter::MultiAttack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		//----------------------------------------------------------------------------- temp solution to rotate character when attacking
		const FRotator ControlRotation = GetControlRotation(); 
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		SetActorRotation(YawRotation);
		//-----------------------------------------------------------------------------
		switch (CharacterState)
		{
		case ECharacterState::ECS_EquippedOneHandedWeapon:
			PlayAttackMontage(true);
			ActionState = EActionState::EAS_MultiAttacking;
			break;
		case ECharacterState::ECS_EquippedTwoHandedWeapon:
			PlayTwoHandedAttackMontage(true);
			ActionState = EActionState::EAS_MultiAttacking;
			break;
		default:
			break;
		}
	}
}

int32 AMainCharacter::PlayAttackMontage(bool isMultiAttack)
{
	Super::PlayAttackMontage(isMultiAttack);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (isMultiAttack && AnimInstance && AttackMontage) // Other montage sections are stored in array, multi attack is stored separately. Most likely will create separate AnimMontage for multiattacks
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection("MultiAttack", AttackMontage);
	}
	return -1;
}

void AMainCharacter::PlayTwoHandedAttackMontage(bool isMultiAttack)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (isMultiAttack && AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(TwoHandedAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("MultiHit"), TwoHandedAttackMontage);
	}
	else if (!isMultiAttack && AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(TwoHandedAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("SingleHit"), TwoHandedAttackMontage);	
	}
	
}

void AMainCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AMainCharacter::EquipWeapon(AWeapon* Weapon, const FName &HandSocketName)
{
	switch (Weapon->GetWeaponType())
	{
	case EWeaponType::EWT_OneHanded:
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		break;
	case EWeaponType::EWT_TwoHanded:
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		break;
	default:
		break;
	}

	Weapon->Equip(GetMesh(), HandSocketName, this, this);
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AMainCharacter::AttackEnd() // Calling it in Attack Anim Montage as notify
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMainCharacter::CanAttack()
{
	const bool bCanAttack =
		ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequiped;
	return bCanAttack;
}

bool AMainCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequiped;
}

bool AMainCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequiped &&
		EquippedWeapon;
}

void AMainCharacter::Arm()
{
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
	{
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	else if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
	{
		PlayEquipMontage(FName("EquipTwoHanded"));
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}

void AMainCharacter::Disarm()
{
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHanded)
	{
		PlayEquipMontage(FName("Unequip"));
		CharacterState = ECharacterState::ECS_Unequiped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	else if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHanded)
	{
		PlayEquipMontage(FName("UnequipTwoHanded"));
		CharacterState = ECharacterState::ECS_Unequiped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}	
}

void AMainCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;

}

void AMainCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EWeaponType WeaponType = EquippedWeapon->GetWeaponType();
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AMainCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{	
		EWeaponType WeaponType = EquippedWeapon->GetWeaponType();
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AMainCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMainCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AMainCharacter::InitSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGoldTextCount(0);
				SlashOverlay->SetSoulsTextCount(0);
			}
		}
	}
}

void AMainCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}