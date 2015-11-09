// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "GameStats.h"

int GameStats::s_nNumPlayers = 2;

GameStats::GameStats()
{
}

GameStats::~GameStats()
{
}

void GameStats::SetNumPlayers(int nPlayers)
{
    s_nNumPlayers = nPlayers;
}
