// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bush.generated.h"

UCLASS()
class BOMBERRATS_API ABush : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABush();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void SpawnPowerup();

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;

    float m_fSpawnRoll;

    float m_fDropRoll;
	
};
