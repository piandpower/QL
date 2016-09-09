//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health pickup component")
    class UBoxComponent* BoxComponent;
};
