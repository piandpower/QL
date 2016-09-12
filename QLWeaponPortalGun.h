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
#include "QLPortalGunCompatibleActor.h"
#include "QLWeapon.h"
#include "QLWeaponPortalGun.generated.h"

// constant

UCLASS()
class QL_API AQLWeaponPortalGun : public AQLWeapon
{
    GENERATED_BODY()
public:
    AQLWeaponPortalGun();
    virtual void Fire() override;
    virtual void AltFire() override;
    void CreatePortal();
    virtual void Tick(float DeltaSeconds) override;
protected:
    bool bBluePortalCreated;
    bool bOrangePortalCreated;
    AQLPortalGunCompatibleActor* BluePortal;
    AQLPortalGunCompatibleActor* OrangePortal;
};
