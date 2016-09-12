//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

// terminology: portal gun compatible actor refers to the special
// type of wall that can have portal created on it

#pragma once

#include "QLUtility.h"
#include "GameFramework/Actor.h"
#include "QLPortalGunCompatibleActor.generated.h"

// forward declaration
class AQLWeaponPortalGun;

UCLASS()
class QL_API AQLPortalGunCompatibleActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLPortalGunCompatibleActor();

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
    AQLPortalGunCompatibleActor* TheOTherPortal;
    AQLWeaponPortalGun* PortalOwner;
};
