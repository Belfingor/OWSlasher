// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"


class USlashOverlay;


UCLASS()
class OWSLASHER_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override; //if using BeginPlay() it will init after MainCharacter and will end up passing nullptr

private:
	UPROPERTY(EditDefaultsOnly, Category = "Slash")
	TSubclassOf<USlashOverlay> SlashOverlayClass;
	UPROPERTY()
	USlashOverlay* SlashOverlay;

public:
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }
};
