// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Bush.h"

#include "BombPowerup.h"
#include "RangePowerup.h"
#include "SpeedPowerup.h"

#define SPAWN_CHANCE 0.65f
#define DROP_CHANCE 0.08f

// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    m_pBox->SetCollisionProfileName(TEXT("BlockAll"));

    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);
    m_pMesh->SetRelativeLocation(FVector::ZeroVector);
    m_pMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.6f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BushMesh(TEXT("StaticMesh'/Game/StaticMeshes/bush.bush'"));

    if (BushMesh.Succeeded())
    {
        m_pMesh->SetStaticMesh(BushMesh.Object);
    }

    m_fSpawnRoll = 0.0f;
    m_fDropRoll = 0.0f;

}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();

    m_fSpawnRoll = FMath::FRand();
    m_fDropRoll  = FMath::FRand();
	
    if (m_fSpawnRoll > SPAWN_CHANCE)
    {
        Destroy();
    }
}

// Called every frame
void ABush::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABush::SpawnPowerup()
{
    AActor* pPowerup = 0;
    FVector vLoc = GetActorLocation();
    vLoc.Z = 50.0f;

    if (m_fSpawnRoll < DROP_CHANCE)
    {
        pPowerup = GetWorld()->SpawnActor<ABombPowerup>(ABombPowerup::StaticClass(), vLoc, FRotator::ZeroRotator);
    }
    else if (m_fSpawnRoll < DROP_CHANCE*2.0f)
    {
        pPowerup = GetWorld()->SpawnActor<ARangePowerup>(ARangePowerup::StaticClass(), vLoc, FRotator::ZeroRotator);
    }
    else if (m_fSpawnRoll < DROP_CHANCE*3.0f)
    {
        pPowerup = GetWorld()->SpawnActor<ASpeedPowerup>(ASpeedPowerup::StaticClass(), vLoc, FRotator::ZeroRotator);
    }
}