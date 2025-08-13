// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercent(float Value)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Value);
	}
}

void USlashOverlay::SetStaminaBarPercent(float Value)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Value);
	}
}

void USlashOverlay::SetGoldTextCount(int32 Value)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
	}
}

void USlashOverlay::SetSoulsTextCount(int32 Value)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
	}
}
