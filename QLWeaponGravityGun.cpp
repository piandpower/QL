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
    bIsGravityGunCompatibleActorHeld = false;
    bIsAltFireHeldDown = false;
    SetCrosshairTexture(TEXT("/Game/Textures/Crosshair/gravity_gun_crosshair"));
    RunningTimeAltFirePressed = 0.0f;
    FixedIntervalAltFirePressed = 0.2f;
    RunningTimeAltFireHeldDown = 0.0f;
    FixedIntervalAltFireHeldDown = 0.2f;
    ggcActor = nullptr;
}

void AQLWeaponGravityGun::Fire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("gravity gun fire")));
}

// If the actor that is compatible with gravity gun is far away,
// AltFire() will exert attractive force on it, until it becomes
// next to the player, when the player starts to hold it.
// When the actor is being held by the player, AltFire() will cause
// it to be released.
//
// Note that:
// bool bIsAltFireHeldDown; // key/button is held down for some time
// bool bIsAltFirePressed; // key/button is pressed, regardless if it is subsequently released or held down
void AQLWeaponGravityGun::AltFire()
{
    bIsAltFirePressed = true;

    // the actor is being held by the player
    if (bIsGravityGunCompatibleActorHeld)
    {
        // only release the actor when the player is making a single click instead of
        // holding down alt fire
        if (!bIsAltFireHeldDown)
        {
            // ggcActor is supposed to be non-null, but we check it just in case
            if (ggcActor)
            {
                UBoxComponent* comp = ggcActor->BoxComponent;

                // by design, the gravity gun compatible actor must have a component
                // here the component is checked for the sake of safety
                if (comp)
                {
                    // character releases the actor
                    bIsGravityGunCompatibleActorHeld = false;

                    // reenable collision between character and component
                    // enum members are shown in engine source code: EngineTypes.h
                    comp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

                    // reenable component physics when the player is holding it
                    comp->SetSimulatePhysics(true);
                }
            }
        }
    }
    // the actor is not being held by the player
    else
    {
        FHitResult hit = Owner->RayTraceFromCharacterPOV();
        // if hit occurs
        if (hit.bBlockingHit)
        {
            // check if the hit actor compatible with (i.e. responsive to) gravity gun
            // if it is not, the result of cast is nullptr
            // note: this is a convenient runtime polymorphic type check!!
            AActor* hitActor = hit.GetActor();
            ggcActor = Cast<AQLGravityGunCompatibleActor>(hitActor);
            if (ggcActor)
            {
                UBoxComponent* comp = ggcActor->BoxComponent;

                // by design, the gravity gun compatible actor must have a component
                // here the component is checked for the sake of safety
                if (comp)
                {
                    // if the compatible actor is considered next to the actor
                    if (Owner->IsObjectNextToCharacter(ggcActor))
                    {
                        // character holds the actor
                        bIsGravityGunCompatibleActorHeld = true;

                        // get rotation delta between character and component
                        FRotator ggcActorRotation = comp->GetComponentRotation();
                        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
                        FRotator cameraRotation = cm->GetCameraRotation();
                        DeltaRotation = ggcActorRotation - cameraRotation;
                        DeltaRotation.Normalize();

                        // disable collision between character and component
                        // enum members are shown in engine source code: EngineTypes.h
                        comp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

                        // disable component physics when the player is holding it
                        comp->SetSimulatePhysics(false);

                        // consequently player then holds the actor for every tick
                        // refer to Tick()
                    }
                    else
                    {
                        // apply attractive force to the component
                        FVector Impulse = -attractiveForceMultFactor * comp->GetMass() * Owner->GetActorForwardVector();
                        comp->AddImpulse(Impulse);
                    }
                }
            }
        }
    }
}

void AQLWeaponGravityGun::AltFireReleased()
{
    bIsAltFireHeldDown = false;
    bIsAltFirePressed = false;
}

void AQLWeaponGravityGun::AltFireRepeat()
{
    AltFire();
}

// In Tick() several things are done:
// --- if alt fire is pressed, check if it is being held down
// --- if alt fire is being held down, call AltFireRepeat() at a constant interval
// --- if gravity gun compatible actor is being held, move it along with the character
void AQLWeaponGravityGun::Tick(float DeltaSeconds)
{
    if (bIsAltFirePressed)
    {
        RunningTimeAltFirePressed += DeltaSeconds;
        if (RunningTimeAltFirePressed >= FixedIntervalAltFirePressed)
        {
            bIsAltFireHeldDown = true;
            RunningTimeAltFirePressed = 0.0f;
        }
    }

    if (bIsAltFireHeldDown)
    {
        // If AltFireRepeat() is called for every tick, the attracted object will gain
        // momentum in a riduculous rapid fashion. As a workaround, AltFireRepeat() is
        // called every FixedInterval.
        RunningTimeAltFireHeldDown += DeltaSeconds;
        if (RunningTimeAltFireHeldDown >= FixedIntervalAltFireHeldDown)
        {
            AltFireRepeat();
            RunningTimeAltFireHeldDown = 0.0f;
        }
    }

    if (bIsGravityGunCompatibleActorHeld)
    {
        if (ggcActor)
        {
            // FIX ME :o(
            // not behave as expected. actor's rotation relative to character is not a fixed vector.
            APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
            FVector newLocation = cm->GetCameraLocation() + cm->GetActorForwardVector() * distanceFromCharacterToActorWhenHold;
            FRotator newRotation = cm->GetCameraRotation() + DeltaRotation;
            ggcActor->SetActorLocation(newLocation);
            ggcActor->SetActorRotation(newRotation);
        }
    }
}