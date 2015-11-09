// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "ExplodeRay.h"
#include "Rat.h"
#include "Bush.h"
#include "Bomb.h"
#include "FakePawn.h"
#include "GameData.h"
#include "Powerup.h"

#define EXPLODE_RAY_LIFETIME 0.3f
#define FADE_TIME 0.1f

// Sets default values
AExplodeRay::AExplodeRay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Find assets
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofMesh(TEXT("StaticMesh'/Game/StaticMeshes/Shape_Cube.Shape_Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat(TEXT("Material'/Game/Materials/M_Ray.M_Ray'"));

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("NoCollision"));

    if (ofMesh.Succeeded() &&
        ofMat.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofMesh.Object);
        m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
        m_pParentMat = ofMat.Object;
    }

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    m_pBox->SetCollisionProfileName(TEXT("OverlapAll"));
    m_pBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);

    m_fTime = EXPLODE_RAY_LIFETIME;
}

// Called when the game starts or when spawned
void AExplodeRay::BeginPlay()
{
	Super::BeginPlay();

    m_pMatInst = UMaterialInstanceDynamic::Create(m_pParentMat, this);
    m_pMesh->SetMaterial(0, m_pMatInst);
	
}

// Called every frame
void AExplodeRay::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if (m_nHit == 0)
    {
        CheckHit();
    }

    m_fTime -= DeltaTime;

    // Removing translucency because it looks bad, I could remove the dynamic material instance
    // and use the a static material instance / material instead, but im leaving it as a dynamic
    // instance incase I want to fix the way the opacity blends.
    //m_pMatInst->SetScalarParameterValue(TEXT("Opacity"), FMath::Clamp(m_fTime/FADE_TIME, 0.0f, 1.0f));

    // Destroy the exploderay if it has faded out
    if (m_fTime < 0.0f)
    {
        Destroy();
    }
}

void AExplodeRay::CheckHit()
{
    TArray<AActor*> arActors;

    m_pBox->GetOverlappingActors(arActors);
    //GetOverlappingActors(arActors);

    for (int32 i = 0; i < arActors.Num(); i++)
    {
        if (Cast<ARat>(arActors[i]) != 0)
        {
            int nPlayerNum = Cast<ARat>(arActors[i])->m_nPlayerNum;
            APlayerController* pCont = UGameplayStatics::GetPlayerController(this,nPlayerNum);
            APawn*        pFakePawn = 0;
            ACameraActor* pCamera   = 0;
            
            for (TActorIterator<AFakePawn> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	        {
		        pFakePawn = Cast<APawn>(*ActorItr);
                break;
	        }
            
            for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	        {
                if (ActorItr->GetName().Equals("MainCamera"))
                {
                     pCamera = *ActorItr;
                     break;
                }
	        }

            if (pFakePawn == 0)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, TEXT("Fake Pawn is NULL"));
            }

            if (pCamera == 0)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, TEXT("Camera is NULL"));
            }

            if (nPlayerNum == 0)
            {
                pCont->Possess(pFakePawn);
                pCont->SetViewTarget(pCamera);
            }
            else
            {
                pCont->UnPossess();
            }

            // Destroy rat actor
            arActors[i]->Destroy();

            // Mark rat as dead
            UGameData::arRatsAlive[nPlayerNum] = 0;
        }

        if (Cast<ABush>(arActors[i]) != 0)
        {
            Cast<ABush>(arActors[i])->SpawnPowerup();
            arActors[i]->Destroy();
        }

        if (Cast<ABomb>(arActors[i]) != 0)
        {
            Cast<ABomb>(arActors[i])->Explode();
        }

        if (Cast<APowerup>(arActors[i]) != 0)
        {
            arActors[i]->Destroy();
        }
    }

    m_nHit = 1;
}

