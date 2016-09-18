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
    SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    SoundComponentList.Add("Fire", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/portal_created"), TEXT("SoundFireComp")));

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
            // remove portal of the same color that is generated elsewhere
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
            // and these two actors should have an almost coincident surface
            FVector location = Hit.ImpactPoint;
            FRotator rotation = UKismetMathLibrary::MakeRotFromXZ(Hit.Normal, pgcActor->GetActorUpVector());
            FTransform transform;
            transform.SetLocation(location);
            transform.SetRotation(rotation.Quaternion());

            AQLPortal* Portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
            Portal->SetQLOwner(this);
            // budge the portal so that it only overlaps the wall a wee bit
            // to allow decal display
            location += (Portal->BoxComponent->GetUnscaledBoxExtent().X - 0.05f) * Hit.Normal;
            Portal->SetActorLocation(location);
            UGameplayStatics::FinishSpawningActor(Portal, transform);

            // the newly spawned portal has top priority
            // previously spawned portal is now destroyed
            TSet<AActor*> AActorList;
            Portal->GetOverlappingActors(AActorList, AQLPortal::StaticClass());
            if (AActorList.Num() > 0)
            {
                for (auto& Item : AActorList)
                {
                    if (Item == BluePortal)
                    {
                        BluePortal->UnsetPortal();
                        BluePortal = nullptr;
                    }
                    else
                    {
                        OrangePortal->UnsetPortal();
                        OrangePortal = nullptr;
                    }
                    Item->Destroy();
                }
            }

            // now that a new portal is appropriately created without overlap,
            // set the new portal's properties
            // esp, set spouse and set spouse's spouse
            if (PortalType == EPortalType::Blue)
            {
                Portal->SetPortal(EPortalType::Blue, OrangePortal);
                BluePortal = Portal;
            }
            else
            {
                Portal->SetPortal(EPortalType::Orange, BluePortal);
                OrangePortal = Portal;
            }

            // apply sound
            PlaySoundComponent("Fire");
        }
    }
    else
    {
        // apply sound
        PlaySoundComponent("None");
    }

    QueryPortal();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::ResetWeapon()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::QueryPortal()
{
    FString BluePortalStatus = BluePortal ? "blue portal on" : "blue portal off";
    FString OrangePortalStatus = OrangePortal ? "orange portal on" : "orange portal off";
    QLUtility::QLSay(BluePortalStatus + "     " + OrangePortalStatus);
}