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
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "QLWeapon.generated.h"

// forward declaration
class AQLCharacter;

UCLASS()
class QL_API AQLWeapon : public AActor
{
    GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AQLWeapon();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void Fire() {};
    virtual void FireReleased() {};
    virtual void FireRepeat() {};

    virtual void AltFire() {};
    virtual void AltFireReleased() {};
    virtual void AltFireRepeat() {};

    AQLCharacter* GetWeaponOwner()  const;
    void SetWeaponOwner(AQLCharacter* WeaponOwner);

    void SetCurrentCrosshairTexture(const FName& crosshairTextureName);
    UTexture2D* GetCurrentCrosshairTexture()  const;
    UTexture2D* CreateCrosshairTexture(const TCHAR* texturePath);
    UAudioComponent* CreateWeaponSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName);
    void PlayWeaponSound(const FName& soundName);
    const FName& GetWeaponName() const;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMeshComponent")
    UStaticMeshComponent* StaticMeshComponent;

protected:
    FName Name;
    bool bIsFireHeldDown;
    bool bIsAltFireHeldDown; // key/button is held down for some time
    bool bIsAltFirePressed; // key/button is pressed, regardless if it is subsequently released or held down
    AQLCharacter* WeaponOwner;
    FHitResult Hit;
    TMap<FName, UAudioComponent*> WeaponSoundList;
    UTexture2D* CurrentCrosshairTexture;
    TMap<FName, UTexture2D*> CrosshairTextureList;
};
