// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "FakePawn.generated.h"

UCLASS()
class BOMBERRATS_API AFakePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFakePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void OnPressStart();

    void OnPressShoulderLeft();

    void OnPressShoulderRight();

    void UpdateMenuRats();

    int m_nEnableControl;

    UFUNCTION(BlueprintCallable, Category = "Bomber Rats")
    static void SetMenuControl(int32 Enable);

    static int s_nEnableMenuControl;

    AActor* m_pRat3;
    AActor* m_pRat4;

	
	
};
