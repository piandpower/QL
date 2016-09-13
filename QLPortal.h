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
#include "QLPortal.generated.h"

// forward declaration
class AQLWeaponPortalGun;

UCLASS()
class QL_API AQLPortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLPortal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity gun compatible actor")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity gun compatible actor")
    UStaticMeshComponent* StaticMeshComponent;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    void SetPortalOwner(AQLWeaponPortalGun* PortalOwner);

protected:
    AQLPortal* TheOTherPortal;
    AQLWeaponPortalGun* PortalOwner;
};
