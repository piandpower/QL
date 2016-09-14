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

#include "QLActor.h"
#include "QLPortal.generated.h"

// forward declaration
class AQLWeaponPortalGun;

UENUM()
enum class EPortalType : uint8
{
    Blue,
    Orange,
};

// portal owner is the portal gun
UCLASS()
class QL_API AQLPortal : public AQLActor
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

    virtual void SetQLOwner(AActor* QLOwner) override;
    virtual void UnSetQLOwner() override;
    AQLWeaponPortalGun* GetPortalOwner();
protected:
    AQLPortal* TheOTherPortal;
    AQLWeaponPortalGun* PortalOwner;
};
