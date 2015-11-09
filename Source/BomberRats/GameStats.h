// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class BOMBERRATS_API GameStats
{
public:
	GameStats();
	~GameStats();
    
    UFUNCTION(BlueprintCallable)
    static void SetNumPlayers(int nPlayers);

    UPROPERTY(BlueprintReadWrite)
    static int s_nNumPlayers;
};
