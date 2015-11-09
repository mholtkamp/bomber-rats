// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "RangePowerup.h"
#include "Rat.h"


ARangePowerup::ARangePowerup()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/explode_powerup.explode_powerup'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("NoCollision"));

    if (ofMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        m_pMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
    }
    
    m_pMesh->AttachTo(RootComponent);
}

void ARangePowerup::Activate(void* pRat)
{
    ARat* pTheRat = reinterpret_cast<ARat*>(pRat);

    if (pTheRat != 0)
    {
        pTheRat->IncreaseBombRange();
    }
}