// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Powerup.h"
#include "RangePowerup.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERRATS_API ARangePowerup : public APowerup
{
	GENERATED_BODY()
	
	ARangePowerup();

    void Activate(void* pRat) override;
	
	
};
