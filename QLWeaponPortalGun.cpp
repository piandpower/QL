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

    // sound
    WeaponSoundList.Add("None", CreateWeaponSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    WeaponSoundList.Add("Fire", CreateWeaponSoundComponent(RootComponent, TEXT("/Game/Sounds/portal_created"), TEXT("SoundFireComp")));
    WeaponSoundList.Add("Teleport", CreateWeaponSoundComponent(RootComponent, TEXT("/Game/Sounds/teleport"), TEXT("SoundTeleportComp")));

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
    Hit = WeaponOwner->RayTraceFromCharacterPOV();
    // if hit occurs
    if (Hit.bBlockingHit)
    {
        // check if the hit actor compatible with (i.e. responsive to) portal gun
        AQLPortalGunCompatibleActor* pgcActor = Cast<AQLPortalGunCompatibleActor>(Hit.GetActor());
        if (pgcActor)
        {
            // remove portal of the same color
            if (PortalType == EPortalType::Blue)
            {
                if (BluePortal)
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
            FVector location = Hit.ImpactPoint;
            //FRotator rotation = FMatrix(Hit.Normal).Rotator();
            FTransform transform;
            transform.SetLocation(location);
            //transform.SetRotation(rotation);

            QLUtility::QLSayLong("--> normal = " + Hit.Normal.ToString());
            QLUtility::QLSayLong("--> pgcActor rot = " + pgcActor->GetActorRotation().ToString());

            AQLPortal* Portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
            Portal->SetQLOwner(this);
            UGameplayStatics::FinishSpawningActor(Portal, transform);

            // the newly spawned portal has top priority
            // previously spawned portal is destroyed
            TSet<AActor*> AActorList;
            Portal->GetOverlappingActors(AActorList, AQLPortal::StaticClass());
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

            // now that a new portal is appropriately created without overlap,
            // set the new portal's properties
            if (PortalType == EPortalType::Blue)
            {
                Portal->StaticMeshComponent->SetMaterial(0, Portal->BluePortalMaterial);
                Portal->SetSpouse(OrangePortal);
                BluePortal = Portal;
            }
            else
            {
                Portal->StaticMeshComponent->SetMaterial(0, Portal->OrangePortalMaterial);
                Portal->SetSpouse(BluePortal);
                OrangePortal = Portal;
            }

            // at last, if spouse exists, inform him/her of myself
            AQLPortal* Spouse = Portal->GetSpouse();
            if (Spouse)
            {
                Spouse->SetSpouse(Portal);
            }

            // apply sound
            PlayWeaponSound("Fire");
        }
    }
    else
    {
        // apply sound
        PlayWeaponSound("None");
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}