// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QLWeapon.h"
#include "QLWeaponPortalGun.generated.h"

UCLASS()
class QL_API AQLWeaponPortalGun : public AQLWeapon
{
    GENERATED_BODY()

private:
    bool bIsHoldingObject;

public:
    AQLWeaponPortalGun();
    virtual void Fire() override;
    virtual void AltFire() override;
};
