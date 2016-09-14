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
    CreatePortal(EPortalType::Blue);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::AltFire()
{
    CreatePortal(EPortalType::Orange);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::CreatePortal(EPortalType PortalType)
{
    AQLPortal* portal;
    if (PortalType == EPortalType::Blue)
    {
        portal = BluePortal;
    }
    else if (PortalType == EPortalType::Orange)
    {
        portal = OrangePortal;
    }

    FVector location = WeaponOwner->QLCameraComponent->GetComponentLocation() + WeaponOwner->QLCameraComponent->GetForwardVector() * 400.0f;
    FTransform transform;
    transform.SetLocation(location);

    portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
    portal->SetQLOwner(this);
    UGameplayStatics::FinishSpawningActor(portal, transform);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}