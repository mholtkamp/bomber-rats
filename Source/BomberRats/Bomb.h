// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class BOMBERRATS_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void Explode();

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    UFUNCTION()
    void OnOverlapEnd(AActor* pOther);

    float m_fTime;

    float m_fScale;

    float m_fRed;

    int m_nOverlappedPawns;

    int m_nCollision;

    int m_nRange;

    void* m_pRatOwner;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    UPROPERTY(EditAnywhere)
    UMaterialInstanceDynamic* m_pMatInst;

    UPROPERTY(EditAnywhere)
    UMaterial* m_pParentMat;
	
};
