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
    AQLActor();
    virtual void BeginPlay() override;
    virtual void Tick( float DeltaSeconds ) override;
    virtual void SetQLOwner(AActor* QLOwner);
    virtual void UnSetQLOwner();
    AActor* GetQLOwner();

    //------------------------------------------------------------
    // play attenuated sound at component's location
    //------------------------------------------------------------
    void PlaySound(const FName& SoundName);

    //------------------------------------------------------------
    // play attenuated sound at given location
    //------------------------------------------------------------
    void PlaySound(const FName& SoundName, const FVector& Location);

    //------------------------------------------------------------
    // play non-attenuated sound at component's location
    //------------------------------------------------------------
    void PlaySound2D(const FName& SoundName);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box component")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static mesh component")
    UStaticMeshComponent* StaticMeshComponent;
protected:
    AActor* QLOwner;
    USoundAttenuation* SoundNoAttenuation;
    USoundAttenuation* SoundAttenuation;
    TMap<FName, UAudioComponent*> SoundList;
    UAudioComponent* CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName);
};
