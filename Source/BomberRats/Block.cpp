// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Block.h"


ABlock::ABlock()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/StaticMeshes/Shape_Cube.Shape_Cube'"));

    if (BlockAsset.Succeeded() &&
        GetStaticMeshComponent() != 0)
    {
        GetStaticMeshComponent()->SetStaticMesh(BlockAsset.Object);
    }
}

