// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "MenuGameMode.h"
#include "FakePawn.h"

AMenuGameMode::AMenuGameMode()
{
    DefaultPawnClass = AFakePawn::StaticClass();
}

