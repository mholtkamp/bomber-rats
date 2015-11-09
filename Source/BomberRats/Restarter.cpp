// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Restarter.h"


// Sets default values
ARestarter::ARestarter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARestarter::BeginPlay()
{
	Super::BeginPlay();

    m_fTimeRemaining = 3.0f;
	
}

// Called every frame
void ARestarter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTimeRemaining -= DeltaTime;

    if (m_fTimeRemaining <= 0.0f)
    {
        UGameplayStatics::OpenLevel(this, TEXT("MenuMap"));
    }

}

