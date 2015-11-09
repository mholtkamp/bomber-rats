// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplodeRay.generated.h"

UCLASS()
class BOMBERRATS_API AExplodeRay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplodeRay();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void CheckHit();

	float m_fTime;

    int m_nHit;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UMaterial* m_pParentMat;

    UPROPERTY(EditAnywhere)
    UMaterialInstanceDynamic* m_pMatInst;
	
};
