// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Powerup.h"
#include "BombPowerup.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERRATS_API ABombPowerup : public APowerup
{
	GENERATED_BODY()
	
public:

	ABombPowerup();
	
    virtual void Activate(void* pRat) override;
};
