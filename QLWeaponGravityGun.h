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

#include "QLGravityGunCompatibleActor.h"
#include "QLWeapon.h"
#include "QLWeaponGravityGun.generated.h"

// constant
const float attractiveForceMultFactor = 1000.0f;
const float repulsiveForceMultFactor = 2000.0f;
const float distanceFromCharacterToActorWhenHold = 400.0f;

UCLASS()
class QL_API AQLWeaponGravityGun : public AQLWeapon
{
    GENERATED_BODY()
public:
    AQLWeaponGravityGun();
    virtual void Fire() override;
    virtual void AltFire() override;
    virtual void AltFireReleased() override;
    virtual void AltFireRepeat() override;
    virtual void Tick(float DeltaSeconds) override;
protected:
    bool bIsGravityGunCompatibleActorHeld;
    float RunningTimeAltFirePressed;
    float FixedIntervalAltFirePressed;
    float RunningTimeAltFireHeldDown;
    float FixedIntervalAltFireHeldDown;
    FRotator DeltaRotation;
    AQLGravityGunCompatibleActor* ggcActor;
};
