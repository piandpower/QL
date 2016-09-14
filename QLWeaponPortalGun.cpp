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

    BluePortal = nullptr;
    OrangePortal = nullptr;
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
    // remove portal of the same color
    if (PortalType == EPortalType::Blue)
    {
        if(BluePortal)
        {
            BluePortal->Destroy();
            BluePortal = nullptr;
        }
    }
    else
    {
        if (OrangePortal)
        {
            OrangePortal->Destroy();
            OrangePortal = nullptr;
        }
    }

    // defer-spawn portal
    FVector location = WeaponOwner->QLCameraComponent->GetComponentLocation() + WeaponOwner->QLCameraComponent->GetForwardVector() * 400.0f;
    FTransform transform;
    transform.SetLocation(location);

    AQLPortal* Portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
    Portal->SetQLOwner(this);
    UGameplayStatics::FinishSpawningActor(Portal, transform);

    // set portal material
    if (PortalType == EPortalType::Blue)
    {
        Portal->StaticMeshComponent->SetMaterial(0, Portal->BluePortalMaterial);
        BluePortal = Portal;
    }
    else
    {
        Portal->StaticMeshComponent->SetMaterial(0, Portal->OrangePortalMaterial);
        OrangePortal = Portal;
    }

    // remove overlapped portal if any
    TSet<AActor*> AActorList;
    TSubclassOf<AQLPortal> ClassFilter;
    Portal->GetOverlappingActors(AActorList, ClassFilter);
    if (AActorList.Num() > 0)
    {
        for (auto& Item : AActorList)
        {
            if (Item == BluePortal)
            {
                // reset blue portal
                BluePortal = nullptr;
            }
            else
            {
                // reset orange portal
                OrangePortal = nullptr;
            }
            Item->Destroy();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}