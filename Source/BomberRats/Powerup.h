// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Powerup.generated.h"

UCLASS()
class BOMBERRATS_API APowerup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Activate(void* pRat);

    UFUNCTION()
    void OnOverlapBegin(AActor* pOther);

    float m_fTime;

    FVector m_vPosition;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_pBox;
	
};
