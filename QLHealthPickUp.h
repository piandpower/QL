// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QLUtility.h"
#include "GameFramework/Actor.h"
#include "QLHealthPickUp.generated.h"

UCLASS()
class QL_API AQLHealthPickUp : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLHealthPickUp();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBeginForComp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity gun compatible actor")
    UBoxComponent* BoxComponent;
};
