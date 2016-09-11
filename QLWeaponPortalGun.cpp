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

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun::AQLWeaponPortalGun()
{
    Name = "portal gun";
    SetCrosshairTexture(TEXT("/Game/Textures/Crosshair/portal_gun_crosshair_original_empty_processed"));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Fire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("portal gun fire")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::AltFire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("portal gun alt fire")));
}