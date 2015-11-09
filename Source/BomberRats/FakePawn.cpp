// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "FakePawn.h"
#include "GameData.h"
#include "Rat.h"

int AFakePawn::s_nEnableMenuControl = 0;

// Sets default values
AFakePawn::AFakePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFakePawn::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<ARat> it(GetWorld()); it; ++it)
    {
        if (it->GetName().Equals("Rat12"))
        {
            m_pRat3 = *it;
        }

        if (it->GetName().Equals("Rat14"))
        {
            m_pRat4 = *it;
        }
    }
	
}

// Called every frame
void AFakePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    

}

// Called to bind functionality to input
void AFakePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAction("MenuStart",EInputEvent::IE_Pressed, this, &AFakePawn::OnPressStart);
    InputComponent->BindAction("MenuInc",EInputEvent::IE_Pressed, this, &AFakePawn::OnPressShoulderRight);
    InputComponent->BindAction("MenuDec",EInputEvent::IE_Pressed, this, &AFakePawn::OnPressShoulderLeft);

}


void AFakePawn::OnPressStart()
{    
    if (s_nEnableMenuControl != 0)
    {
        UGameData::ReviveRats();
        UGameplayStatics::OpenLevel(this, TEXT("BomberMap"));
        s_nEnableMenuControl = 0;
    }
}

void AFakePawn::OnPressShoulderLeft()
{
    if (s_nEnableMenuControl != 0)
    {
        UGameData::NumRats--;

        if (UGameData::NumRats < 2)
        {
            UGameData::NumRats = 2;
        }

        UpdateMenuRats();
    }
}

void AFakePawn::OnPressShoulderRight()
{
    if (s_nEnableMenuControl != 0)
    {
        UGameData::NumRats++;

        if (UGameData::NumRats > 4)
        {
            UGameData::NumRats = 4;
        }

        UpdateMenuRats();
    }
}


void AFakePawn::UpdateMenuRats()
{
    if (m_pRat3 != 0 &&
        m_pRat4 != 0 &&
        s_nEnableMenuControl != 0)
    {
        if (UGameData::NumRats >= 3)
        {
            m_pRat3->SetActorHiddenInGame(0);
        }
        else
        {
            m_pRat3->SetActorHiddenInGame(1);
        }

        if (UGameData::NumRats >= 4)
        {
            m_pRat4->SetActorHiddenInGame(0);
        }
        else
        {
            m_pRat4->SetActorHiddenInGame(1);
        }
    }
}

void AFakePawn::SetMenuControl(int32 Enable)
{
    s_nEnableMenuControl = Enable;
}
