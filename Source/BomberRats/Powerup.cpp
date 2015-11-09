// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Powerup.h"
#include "Rat.h"

#define SPIN_RATE 110.0f
#define BOB_AMPLITUDE 30.0f
#define BOB_RATE 5.0f

// Sets default values
APowerup::APowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    OnActorBeginOverlap.AddDynamic(this, &APowerup::OnOverlapBegin);

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->SetCollisionProfileName(TEXT("No Collision"));
    m_pBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    m_pBox->InitBoxExtent(FVector(30.0f, 30.0f, 30.0f));
    m_pBox->bGenerateOverlapEvents = 1;

    RootComponent = m_pBox;

    m_fTime = 0.0f;
}

// Called when the game starts or when spawned
void APowerup::BeginPlay()
{
	Super::BeginPlay();

    m_vPosition = GetActorLocation();
	
}

// Called every frame
void APowerup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTime += DeltaTime;

    SetActorRotation(FRotator(0.0f, m_fTime * SPIN_RATE, 0.0f));
    SetActorLocation(m_vPosition + FVector(0.0f, 0.0f, BOB_AMPLITUDE * FMath::Sin(m_fTime * BOB_RATE)));

}

void APowerup::Activate(void* pRat)
{
    
}

void APowerup::OnOverlapBegin(AActor* pOther)
{
    if(Cast<ARat>(pOther) != 0)
    {
        // If it's a rat, activate the powerup and then destroy self.
        Activate(pOther);

        Destroy();
    }
}