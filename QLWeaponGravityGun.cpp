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
#include "QLWeaponGravityGun.h"
#include "QLCharacter.h"

AQLWeaponGravityGun::AQLWeaponGravityGun()
{
    Name = "gravity gun";
    bIsHoldingObject = false;
    SetCrosshairTexture(TEXT("/Game/Textures/Crosshair/gravity_gun_crosshair"));
}

void AQLWeaponGravityGun::Fire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("gravity gun fire")));
}

void AQLWeaponGravityGun::AltFire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("gravity gun alt fire")));
    bIsAltFireHeldDown = true;

    owner->RayTrace();
}

void AQLWeaponGravityGun::AltFireReleased()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("gravity gun alt fire released")));
    bIsAltFireHeldDown = false;
}

void AQLWeaponGravityGun::AltFireRepeat()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("gravity gun alt fire repeated")));
}

void AQLWeaponGravityGun::Tick(float DeltaSeconds)
{
    if (true == bIsAltFireHeldDown)
    {
        AltFireRepeat();
    }
}