// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Powerup.h"
#include "SpeedPowerup.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERRATS_API ASpeedPowerup : public APowerup
{
	GENERATED_BODY()
	
	ASpeedPowerup();

    void Activate(void* pRat);
};
