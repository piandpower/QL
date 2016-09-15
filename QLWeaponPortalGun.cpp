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
            // the portal should be located outside the portal gun compatible actor
            // and these two actors should have a coincident surface
            FVector location = Hit.ImpactPoint;
            FRotator rotation = Hit.Normal.Rotation();
            // equivalently: UKismetMathLibrary::MakeRotFromX(Hit.Normal);
            FTransform transform;
            transform.SetLocation(location);
            transform.SetRotation(rotation.Quaternion());

            AQLPortal* Portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
            Portal->SetQLOwner(this);
            // Move the portal a little bit
            location += Portal->BoxComponent->GetUnscaledBoxExtent().X * Hit.Normal;
            Portal->SetActorLocation(location);
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
                Portal->SetPortalForwardVector(Hit.Normal);
                BluePortal = Portal;
            }
            else
            {
                Portal->StaticMeshComponent->SetMaterial(0, Portal->OrangePortalMaterial);
                Portal->SetSpouse(BluePortal);
                Portal->SetPortalForwardVector(Hit.Normal);
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