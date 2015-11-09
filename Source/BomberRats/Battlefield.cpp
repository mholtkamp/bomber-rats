// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Battlefield.h"
#include "Rat.h"
#include "GameData.h"


// Sets default values
ABattlefield::ABattlefield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (m_pCamera != 0)
    {
        GetWorld()->GetFirstPlayerController()->SetViewTarget(m_pCamera);
    }
}

// Called when the game starts or when spawned
void ABattlefield::BeginPlay()
{
	Super::BeginPlay();

    if (m_pCamera != 0)
    {
        //GetWorld()->GetFirstPlayerController()->UnPossess();
        m_pCont1 = GetWorld()->GetFirstPlayerController();
        m_pCont1->SetViewTarget(m_pCamera);
        m_pRat1 = m_pCont1->GetPawn();
        Cast<ARat>(m_pRat1)->SetPlayerNum(0);

        m_pCont2 = UGameplayStatics::CreatePlayer(this);
        m_pCont2->SetViewTarget(m_pCamera);
        m_pRat2 = m_pCont2->GetPawn();
        Cast<ARat>(m_pRat2)->SetPlayerNum(1);

        if (UGameData::NumRats >= 3)
        {
            m_pCont3 = UGameplayStatics::CreatePlayer(this);
            m_pCont3->SetViewTarget(m_pCamera);
            m_pRat3 = m_pCont3->GetPawn();
            Cast<ARat>(m_pRat3)->SetPlayerNum(2);
        }

        if (UGameData::NumRats >= 4)
        {
            m_pCont4 = UGameplayStatics::CreatePlayer(this);
            m_pCont4->SetViewTarget(m_pCamera);
            m_pRat4 = m_pCont4->GetPawn();
            Cast<ARat>(m_pRat4)->SetPlayerNum(3);
        }
    }
	
}

// Called every frame
void ABattlefield::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    int nPlayersAlive = 0;

    for (int i = 0; i < MAX_RATS; i++)
    {
        nPlayersAlive += UGameData::arRatsAlive[i];
    }

    // If there is only 1 rat alive (or less) then the game is over
    if (nPlayersAlive <= 1)
    {
        // Remove other players
        UGameplayStatics::RemovePlayer(m_pCont2, 1);
        
        if (UGameData::NumRats >= 3)
        {
            UGameplayStatics::RemovePlayer(m_pCont3, 1);
        }
        if (UGameData::NumRats >= 4)
        {
            UGameplayStatics::RemovePlayer(m_pCont4, 1);
        }
        

        UGameplayStatics::OpenLevel(this, TEXT("GameOver"));
    }
}

