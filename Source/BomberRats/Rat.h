// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Rat.generated.h"

UCLASS()
class BOMBERRATS_API ARat : public APawn
{
	GENERATED_BODY()

public:

    enum AnimEnum
    {
        ANIM_IDLE  = 0,
        ANIM_WALK  = 1,
        ANIM_PLANT = 2
    };

	// Sets default values for this pawn's properties
	ARat();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void SetPlayerNum(int nNum);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void IncreaseMaxBombs();

    void IncreaseBombRange();

    void IncreaseSpeed();

    void MoveX(float fAxisValue);

    void MoveY(float fAxisValue);

    void Plant();

    float m_fMoveXVal;
    
    float m_fMoveYVal;

    float m_fSpeed;

    int m_nCurAnim;

    int m_nPlayerNum;

    int m_nMaxBombs;

    int m_nBombsOut;

    int m_nBombRange;

    UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;
	
};
