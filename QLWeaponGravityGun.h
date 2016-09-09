// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QLWeapon.h"
#include "QLWeaponGravityGun.generated.h"

UCLASS()
class QL_API AQLWeaponGravityGun : public AQLWeapon
{
    GENERATED_BODY()
private:
    bool bIsHoldingObject;
public:
    AQLWeaponGravityGun();
    virtual void Fire() override;
    virtual void AltFire() override;
    virtual void AltFireReleased() override;
    virtual void AltFireRepeat() override;
    virtual void Tick(float DeltaSeconds) override;
};
