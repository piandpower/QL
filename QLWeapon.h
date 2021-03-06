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
#include "GameFramework/Character.h"
#include "QLWeapon.generated.h"

// forward declaration
class AQLCharacter;

UCLASS()
class QL_API AQLWeapon : public AQLActor
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

    void InitializeCurrentCrosshairTexture(const FName& crosshairTextureName);
    UTexture2D* CreateCrosshairTexture(const TCHAR* texturePath);
    const FName& GetWeaponName() const;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    virtual void SetQLOwner(AActor* QLOwner) override;
    virtual void UnSetQLOwner() override;
    AQLCharacter* GetWeaponOwner();
    virtual void ResetWeapon() {};
    TArray<UTexture2D*> CurrentCrosshairTextureList;
    TMap<FName, UTexture2D*> CrosshairTextureList;
    virtual void Zoom() {};

protected:
    FName Name;
    bool bIsFireHeldDown;
    bool bIsAltFireHeldDown; // key/button is held down for some time
    bool bIsAltFirePressed; // key/button is pressed, regardless if it is subsequently released or held down
    AQLCharacter* WeaponOwner;
    FHitResult Hit;
};
