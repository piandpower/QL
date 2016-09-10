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

#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "QLWeapon.generated.h"

// forward declaration
class AQLCharacter;

UCLASS()
class QL_API AQLWeapon : public AActor
{
    GENERATED_BODY()

protected:
    FString Name;
    bool bIsFireHeldDown;
    bool bIsAltFireHeldDown; // key/button is held down for some time
    bool bIsAltFirePressed; // key/button is pressed, regardless if it is subsequently released or held down
    AQLCharacter* Owner;
    UTexture2D* CrosshairTexture;

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

    void SetOwner(AQLCharacter* owner);

    UTexture2D* GetCrosshairTexture()  const;
    void SetCrosshairTexture(const TCHAR* textureName);
    const FString& GetWeaponName() const;
};
