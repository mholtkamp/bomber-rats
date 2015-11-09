// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "BomberRatsGameMode.h"
#include "Rat.h"


ABomberRatsGameMode::ABomberRatsGameMode()
{
    DefaultPawnClass = ARat::StaticClass();
}

