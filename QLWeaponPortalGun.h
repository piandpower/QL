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

#include "QLWeapon.h"
#include "QLWeaponPortalGun.generated.h"

UCLASS()
class QL_API AQLWeaponPortalGun : public AQLWeapon
{
    GENERATED_BODY()
public:
    AQLWeaponPortalGun();
    virtual void Fire() override;
    virtual void AltFire() override;
protected:
};
