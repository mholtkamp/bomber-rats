// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Bomb.h"
#include "ExplodeRay.h"
#include "Rat.h"

#define TICK_TIME 3.0f
#define COLLISION_CONVERT_TIME TICK_TIME - 0.3f
#define COLOR_CHANGE_RATE 10.0f
#define SCALE_CHANGE_RATE 10.0f
#define RAY_WIDTH 0.8f

static USoundWave* s_pExplodeSound = 0;

// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ofSphereMesh(TEXT("StaticMesh'/Game/StaticMeshes/Shape_Sphere.Shape_Sphere'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofSphereMat(TEXT("Material'/Game/Materials/M_Bomb.M_Bomb'"));
    static ConstructorHelpers::FObjectFinder<USoundWave> ofExplodeSound(TEXT("SoundWave'/Game/Sounds/boom_16.boom_16'"));
    
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_pMesh->SetCollisionProfileName(TEXT("NoCollision"));
    m_pMesh->SetRelativeLocation(FVector(0.0f,0.0f,-50.0f));

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    m_pBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    m_pBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

    // Set delegates for collision
    OnActorBeginOverlap.AddDynamic(this, &ABomb::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ABomb::OnOverlapEnd);
    
    if (ofSphereMesh.Succeeded() &&
        ofSphereMat.Succeeded() &&
        ofExplodeSound.Succeeded())
    {
        m_pMesh->SetStaticMesh(ofSphereMesh.Object);
        m_pParentMat = ofSphereMat.Object;
        s_pExplodeSound = ofExplodeSound.Object;
    }

    RootComponent = m_pBox;
    m_pMesh->AttachTo(RootComponent);

    m_fTime = TICK_TIME;
    m_fScale = 1.0f;
    m_fRed = 0.0f;
    m_nOverlappedPawns = 0;
    m_nCollision = 0;
    m_nRange = 3;
    m_pRatOwner = 0;
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

    // Create a material instance for this bomb
    m_pMatInst = UMaterialInstanceDynamic::Create(m_pParentMat, this);
    m_pMesh->SetMaterial(0, m_pMatInst);
}

// Called every frame
void ABomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    m_fTime -= DeltaTime;

    // Update the color of material and scale of mesh
    m_fRed = 0.6f + 0.3f*FMath::Sin(m_fTime * COLOR_CHANGE_RATE);
    m_pMatInst->SetVectorParameterValue(TEXT("Color"), FLinearColor(m_fRed, 0.1f, 0.1f, 1.0f));

    // Check for collision conversion
    if (m_fTime < COLLISION_CONVERT_TIME)
    {
        if (m_nOverlappedPawns == 0 &&
            m_nCollision       == 0)
        {
            m_nCollision = 1;
            m_pBox->SetCollisionProfileName(TEXT("BlockAll"));
        }
    }

    // Check to see if it is time to explode
    if (m_fTime <= 0.0f)
    {
        Explode();

        // Destroy the bomb
        Destroy();
    }

}

void ABomb::OnOverlapBegin(AActor* pOther)
{
    m_nOverlappedPawns++;
}

void ABomb::OnOverlapEnd(AActor* pOther)
{
    m_nOverlappedPawns--;
}

void ABomb::Explode()
{
    FHitResult hit;
    bool bHit = 0;
    float fRange = static_cast<float>(m_nRange) * 100.0f;
    float fHitRange = 0.0f;
    FVector vBombLoc = GetActorLocation();

    //@@ DEBUG
    //const FName TraceTag("DebugTrace");
 
    // GetWorld()->DebugDrawTraceTag = TraceTag;
 
    // FCollisionQueryParams CollisionParams;
    // CollisionParams.TraceTag = TraceTag;
    //@@ END

    AActor* arRays[4]; // 4 rays for each direction, up/down/left/right
    
    reinterpret_cast<ARat*>(m_pRatOwner)->m_nBombsOut--;

    UGameplayStatics::PlaySoundAtLocation(this, s_pExplodeSound, vBombLoc);

    Destroy();

    for (uint32 i = 0; i < 4; i++)
    {
        arRays[i] = GetWorld()->SpawnActor(AExplodeRay::StaticClass());
        arRays[i]->SetActorLocation(FVector(0.0f, 0.0f, -5000.0f));

        switch(i)
        {
        case EXP_RIGHT:
            bHit = GetWorld()->LineTraceSingleByChannel(hit, vBombLoc, vBombLoc + FVector(0.0f, -fRange, 0.0f), ECC_GameTraceChannel1, ECC_WorldStatic);
            if (bHit)
            {
                fHitRange = FMath::Abs(vBombLoc.Y - hit.Actor->GetActorLocation().Y);
                arRays[i]->SetActorLocation(vBombLoc + FVector(0.0f, -fHitRange/2.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(RAY_WIDTH, fHitRange/100.0f, RAY_WIDTH));
            }
            else
            {
                arRays[i]->SetActorLocation(vBombLoc + FVector(0.0f, -fRange/2.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(RAY_WIDTH, fRange/100.0f, RAY_WIDTH));
            }
            
            break;
        case EXP_UP:
            bHit = GetWorld()->LineTraceSingleByChannel(hit, vBombLoc, vBombLoc + FVector(-fRange, 0.0f, 0.0f), ECC_GameTraceChannel1, ECC_WorldStatic);
            if (bHit)
            {
                fHitRange = FMath::Abs(vBombLoc.X - hit.Actor->GetActorLocation().X);
                arRays[i]->SetActorLocation(vBombLoc + FVector(-fHitRange/2.0f, 0.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(fHitRange/100.0f, RAY_WIDTH, RAY_WIDTH));
            }
            else
            {
                arRays[i]->SetActorLocation(GetActorLocation() + FVector(-fRange/2.0f, 0.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(fRange/100.0f, RAY_WIDTH, RAY_WIDTH));
            }
            break;
        case EXP_LEFT:
            bHit = GetWorld()->LineTraceSingleByChannel(hit, vBombLoc, vBombLoc + FVector(0.0f, fRange, 0.0f), ECC_GameTraceChannel1, ECC_WorldStatic);
            if (bHit)
            {
                fHitRange = FMath::Abs(vBombLoc.Y - hit.Actor->GetActorLocation().Y);
                arRays[i]->SetActorLocation(vBombLoc + FVector(0.0f, fHitRange/2.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(RAY_WIDTH, fHitRange/100.0f, RAY_WIDTH));
            }
            else
            {
                arRays[i]->SetActorLocation(GetActorLocation() + FVector(0.0f, fRange/2.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(RAY_WIDTH, fRange/100.0f, RAY_WIDTH));
            }
            break;
        case EXP_DOWN:
            bHit = GetWorld()->LineTraceSingleByChannel(hit, vBombLoc, vBombLoc + FVector(fRange, 0.0f, 0.0f), ECC_GameTraceChannel1, ECC_WorldStatic);
            if (bHit)
            {
                fHitRange = FMath::Abs(vBombLoc.X - hit.Actor->GetActorLocation().X);
                arRays[i]->SetActorLocation(vBombLoc + FVector(fHitRange/2.0f, 0.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(fHitRange/100.0f, RAY_WIDTH, RAY_WIDTH));
            }
            else
            {
                arRays[i]->SetActorLocation(GetActorLocation() + FVector(fRange/2.0f, 0.0f, 0.0f));
                arRays[i]->SetActorScale3D(FVector(fRange/100.0f, RAY_WIDTH, RAY_WIDTH));
            }
            break;
        default:
            break;
        }

    }
}
