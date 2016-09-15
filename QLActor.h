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

#include "QLUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "QLActor.generated.h"

UCLASS()
class QL_API AQLActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    AActor* QLOwner;
    virtual void SetQLOwner(AActor* QLOwner);
    virtual void UnSetQLOwner();
    AActor* GetQLOwner();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box component")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static mesh component")
    UStaticMeshComponent* StaticMeshComponent;
};
