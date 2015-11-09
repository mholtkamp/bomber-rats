// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "GameData.h"


int32 UGameData::NumRats = 2;

int32 UGameData::arRatsAlive[4] = {0, 0, 0, 0};

void UGameData::SetNumRats(int32 Num)
{
    NumRats = Num;
}

void UGameData::ReviveRats()
{
    for (int i = 0; i < 4; i++)
        arRatsAlive[i] = 0;

    for (int i = 0; i < NumRats; i++)
        arRatsAlive[i] = 1;
}

bool UGameData::IsRatAlive(int32 nRat)
{
    if (arRatsAlive[nRat] != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}