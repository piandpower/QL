//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLWeaponPortalGun.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun::AQLWeaponPortalGun()
{
    Name = "PortalGun";
    CrosshairTextureList.Add("BothEmpty", CreateCrosshairTexture(TEXT("/Game/Textures/Crosshair/portal_gun_crosshair_original_empty_processed")));
    SetCurrentCrosshairTexture("BothEmpty");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Fire()
{
    // test: create a portal object
    CreatePortal();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::AltFire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::CreatePortal()
{
    FVector location = WeaponOwner->QLCameraComponent->GetComponentLocation() + WeaponOwner->QLCameraComponent->GetForwardVector() * 400.0f;
    BluePortal = GetWorld()->SpawnActor<AQLPortal>(AQLPortal::StaticClass(), location, FRotator::ZeroRotator);
    BluePortal->SetPortalOwner(this);

    QLUtility::QLSayLong(location.ToString());
    QLUtility::QLSayLong(BluePortal->GetActorLocation().ToString());
    QLUtility::QLSayLong(this->GetActorLocation().ToString());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}