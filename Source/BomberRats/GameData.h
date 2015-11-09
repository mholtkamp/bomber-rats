// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "GameData.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERRATS_API UGameData : public UObject
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "Bomber Rats")
	static void SetNumRats(int32 Num);

    UFUNCTION(BlueprintCallable, Category = "Bomber Rats")
    static void ReviveRats();

    UFUNCTION(BlueprintCallable, Category = "Bomber Rats")
    static bool IsRatAlive(int32 nRat);

    static int32 arRatsAlive[4];

	static int32 NumRats;
	
};
