// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberRats.h"
#include "Rat.h"
#include "Bomb.h"

#define MOVE_THRESHOLD 0.4f
#define DEFAULT_SPEED 400.0f
#define DEFAULT_MAX_BOMBS 1
#define DEFAULT_BOMB_RANGE 1

#define MAX_BOMBS_CAP 4
#define BOMB_RANGE_CAP 8
#define SPEED_CAP 1000.0f

#define SPEED_BOOST_INCR 100.0f

static UAnimationAsset* s_pIdleAnim;
static UAnimationAsset* s_pWalkAnim;
static UAnimationAsset* s_pPlantAnim;

static UMaterial* s_pMat1;
static UMaterial* s_pMat2;
static UMaterial* s_pMat3;
static UMaterial* s_pMat4;


// Sets default values
ARat::ARat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    m_pBox  = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

    m_pBox->InitBoxExtent(FVector(30.0f, 30.0f, 90.0f));
    m_pBox->SetCollisionProfileName(TEXT("Pawn"));
    m_pBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
    RootComponent = m_pBox;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ofRatMesh(TEXT("SkeletalMesh'/Game/SkeletalMeshes/Bomber.Bomber'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofIdleAnim(TEXT("AnimSequence'/Game/SkeletalMeshes/Bomber_Anim_Idle.Bomber_Anim_Idle'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofWalkAnim(TEXT("AnimSequence'/Game/SkeletalMeshes/Bomber_Anim_Walk.Bomber_Anim_Walk'"));
    static ConstructorHelpers::FObjectFinder<UAnimSequence> ofPlantAnim(TEXT("AnimSequence'/Game/SkeletalMeshes/Bomber_Anim_Plant.Bomber_Anim_Plant'"));

    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat1(TEXT("Material'/Game/Materials/M_Rat1.M_Rat1'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat2(TEXT("Material'/Game/Materials/M_Rat2.M_Rat2'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat3(TEXT("Material'/Game/Materials/M_Rat3.M_Rat3'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ofMat4(TEXT("Material'/Game/Materials/M_Rat4.M_Rat4'"));

    if (ofRatMesh.Succeeded())
    {
        m_pMesh->SetSkeletalMesh(ofRatMesh.Object);
        m_pMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
        m_pMesh->SetWorldScale3D(FVector(0.65f, 0.65f, 0.5f));
        m_pMesh->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));

        s_pIdleAnim  = ofIdleAnim.Object;
        s_pWalkAnim  = ofWalkAnim.Object;
        s_pPlantAnim = ofPlantAnim.Object;
    }

    if (ofMat1.Succeeded() &&
        ofMat2.Succeeded() &&
        ofMat3.Succeeded() &&
        ofMat4.Succeeded())
    {
        s_pMat1 = ofMat1.Object;
        s_pMat2 = ofMat2.Object;
        s_pMat3 = ofMat3.Object;
        s_pMat4 = ofMat4.Object;
    }
    m_pMesh->AttachTo(RootComponent);

    m_nMaxBombs  = DEFAULT_MAX_BOMBS;
    m_nBombsOut  = 0;
    m_nBombRange = DEFAULT_BOMB_RANGE;
    m_fSpeed     = DEFAULT_SPEED;

    m_fMoveXVal  = 0.0f;
    m_fMoveYVal  = 0.0f;

}

// Called when the game starts or when spawned
void ARat::BeginPlay()
{
	Super::BeginPlay();

    // Start Playing Idle Animation
    //m_pMesh->InitAnim(true);
    //m_pMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    m_nCurAnim = ANIM_IDLE;
    m_pMesh->SetAnimation(s_pIdleAnim);
    m_pMesh->PlayAnimation(s_pIdleAnim, true);
}

// Called every frame
void ARat::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    float fMoveXDir = 0.0f;
    float fMoveYDir = 0.0f;

    float fMoveXAbs = FMath::Abs(m_fMoveXVal);
    float fMoveYAbs = FMath::Abs(m_fMoveYVal);

    FHitResult hit;

    if (fMoveXAbs > MOVE_THRESHOLD ||
        fMoveYAbs > MOVE_THRESHOLD)
    {
        FVector vNewLoc = GetActorLocation();

        if (fMoveXAbs > fMoveYAbs)
        {
            fMoveXDir = FMath::Sign(m_fMoveXVal);
            vNewLoc.X -= fMoveXDir*m_fSpeed*DeltaTime;
        }
        else
        {
            fMoveYDir = FMath::Sign(m_fMoveYVal);
            vNewLoc.Y -= fMoveYDir*m_fSpeed*DeltaTime;
        }

        // Move actor and check for swept collision
        SetActorLocation(vNewLoc, 1, &hit);

        // If the rat collided with a block, try sliding it
        // to assist player movement.
        if (hit.GetActor()                         != 0 &&
            Cast<AStaticMeshActor>(hit.GetActor()) != 0)
        {

            // A collision happened and it was a static mesh actor
            // Make sure it's not a Wall
            if (hit.GetActor()->GetName().Contains(TEXT("Wall")))
            {
                // It was a wall, don't attempt to assist movement.    
            }
            else
            {
                // It was a block, so attempt to slide pawn
                if (fMoveXAbs > fMoveYAbs)
                {
                    float fMod = FMath::Abs(FMath::Fmod(GetActorLocation().Y, 200.0f));
                    
                    if (fMod <= 100.0f &&
                        fMod >  30.0f)
                    {
                         vNewLoc = GetActorLocation();
                         vNewLoc.Y -= m_fSpeed*DeltaTime;
                         SetActorLocation(vNewLoc, 1);
                    }
                    else if (fMod > 100.0f &&
                             fMod < 170.0f)
                    {
                         vNewLoc = GetActorLocation();
                         vNewLoc.Y += m_fSpeed*DeltaTime;
                         SetActorLocation(vNewLoc, 1);
                    }
                }
                else
                {
                    float fMod = FMath::Abs(FMath::Fmod(GetActorLocation().X, 200.0f));

                    if (fMod <= 100.0f &&
                        fMod >  30.0f)
                    {
                         vNewLoc = GetActorLocation();
                         vNewLoc.X += m_fSpeed*DeltaTime;
                         SetActorLocation(vNewLoc, 1);
                    }
                    else if (fMod > 100.0f &&
                             fMod < 170.0f)
                    {
                         vNewLoc = GetActorLocation();
                         vNewLoc.X -= m_fSpeed*DeltaTime;
                         SetActorLocation(vNewLoc, 1);
                    }
                }
            }
            

        }



        // Change animation to walk
        if (m_nCurAnim == ANIM_IDLE)
        {
            m_nCurAnim = ANIM_WALK;
            m_pMesh->SetAnimation(s_pWalkAnim);
            m_pMesh->PlayAnimation(s_pWalkAnim, true);
        }

        // Rotate mesh to walk in correct direction
        if (fMoveXDir < 0.0f)
        {
            m_pMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
        }
        else if (fMoveXDir > 0.0f)
        {
            m_pMesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
        }
        else if (fMoveYDir < 0.0f)
        {
            m_pMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
        }
        else if (fMoveYDir > 0.0f)
        {
            m_pMesh->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
        }
    }
    else
    {
        if (m_nCurAnim == ANIM_WALK)
        {
            m_nCurAnim = ANIM_IDLE;
            m_pMesh->SetAnimation(s_pIdleAnim);
            m_pMesh->PlayAnimation(s_pIdleAnim, true);
        }
    }

    UpdateComponentTransforms();
    UpdateOverlaps();
}

// Called to bind functionality to input
void ARat::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis("MoveX", this, &ARat::MoveX);
    InputComponent->BindAxis("MoveY", this, &ARat::MoveY);
    InputComponent->BindAction("Plant",EInputEvent::IE_Pressed, this, &ARat::Plant);
}

void ARat::MoveX(float fAxisValue)
{
    m_fMoveXVal = fAxisValue;
}

void ARat::MoveY(float fAxisValue)
{
    m_fMoveYVal = fAxisValue;
}

void ARat::Plant()
{
    if (m_nBombsOut < m_nMaxBombs)
    {
        m_nBombsOut++;

        // Find a good, grid-aligned location for the bomb
        FVector vSpawnLoc = GetActorLocation();
        vSpawnLoc.Z = 50.0f;
        vSpawnLoc.X = static_cast<float>(FMath::Round(vSpawnLoc.X/100.0f)) * 100.0f;
        vSpawnLoc.Y = static_cast<float>(FMath::Round(vSpawnLoc.Y/100.0f)) * 100.0f;

        // Spawn the bomb and give it the desired location
        AActor* pBomb = GetWorld()->SpawnActor(ABomb::StaticClass());
        pBomb->SetActorLocation(vSpawnLoc);

        // Set a pointer to owner so when bomb explodes it can
        // decrement the m_nBombsOut variable in rat so the rat 
        // can place another bomb.
        Cast<ABomb>(pBomb)->m_pRatOwner = this;
        Cast<ABomb>(pBomb)->m_nRange = m_nBombRange;

        // Consider adding a plant animation.
        // Would be nice to know how to find out how much time is remaining on an animation.
    }

}

void ARat::SetPlayerNum(int nNum)
{
    m_nPlayerNum = nNum;

    switch (nNum)
    {
    case 0:
        m_pMesh->SetMaterial(0, s_pMat1);
        break;
    case 1:
        m_pMesh->SetMaterial(0, s_pMat2);
        break;
    case 2:
        m_pMesh->SetMaterial(0, s_pMat3);
        break;
    case 3:
        m_pMesh->SetMaterial(0, s_pMat4);
        break;
    default:
        break;
    }
}

void ARat::IncreaseMaxBombs()
{
    m_nMaxBombs++;

    if (m_nMaxBombs > MAX_BOMBS_CAP)
    {
        m_nMaxBombs = MAX_BOMBS_CAP;
    }
}

void ARat::IncreaseBombRange()
{
    m_nBombRange++;

    if (m_nBombRange > BOMB_RANGE_CAP)
    {
        m_nBombRange = BOMB_RANGE_CAP;
    }
}

void ARat::IncreaseSpeed()
{
    m_fSpeed += SPEED_BOOST_INCR;

    if (m_fSpeed > SPEED_CAP)
    {
        m_fSpeed = SPEED_CAP;
    }
}
