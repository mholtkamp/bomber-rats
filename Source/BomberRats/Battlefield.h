// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Battlefield.generated.h"

UCLASS()
class BOMBERRATS_API ABattlefield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattlefield();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    // Camera pointer
    UPROPERTY(EditAnywhere)
    ACameraActor* m_pCamera;

    UPROPERTY(EditAnywhere)
    APawn* m_pRat1;

    UPROPERTY(EditAnywhere)
    APawn* m_pRat2;

    UPROPERTY(EditAnywhere)
    APawn* m_pRat3;

    UPROPERTY(EditAnywhere)
    APawn* m_pRat4;

    UPROPERTY(EditAnywhere)
    APlayerController* m_pCont1;

    UPROPERTY(EditAnywhere)
    APlayerController* m_pCont2;

    UPROPERTY(EditAnywhere)
    APlayerController* m_pCont3;

    UPROPERTY(EditAnywhere)
    APlayerController* m_pCont4;

};
