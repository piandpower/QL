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
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacter::AQLCharacter()
{
    DoubleJumpCounter = 0;

    bIsSprinting = false;
    bWantToSprint = false;
    bAllWeaponUnlockable = true;

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // movement
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 1.0f;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
    MaxWalkSpeedCrouched = 100.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;
    //GetCharacterMovement()->MaxStepHeight = 100.0f;

    // camera
    // note pawn has a member var: BaseEyeHeight
    QLCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    QLCameraComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
    QLCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
    QLCameraComponent->bUsePawnControlRotation = true;

    // physics handle
    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

    // creat all weapon slots
    WeaponList.Add("GravityGun", nullptr);
    WeaponList.Add("PortalGun", nullptr);
    WeaponList.Add("NeutronAWP", nullptr);
    CurrentWeapon = nullptr;
    LastWeapon = nullptr;

    // sound
    SoundList.Add("EquipWeapon", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/medshot4_from_hl2"), TEXT("SoundEquipWeaponComp")));
    SoundList.Add("SwitchWeapon", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/swords_collide"), TEXT("SoundSwitchWeaponComp")));
    SoundList.Add("DoubleJump", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/quake_jump"), TEXT("SoundDoubleJumpComp")));

    SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    SoundNoAttenuation->Attenuation.bAttenuate = false;

    SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    SoundAttenuation->Attenuation.bAttenuate = true;
    SoundAttenuation->Attenuation.DistanceAlgorithm = ESoundDistanceModel::ATTENUATION_Linear;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter::~AQLCharacter()
{
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCharacter::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLCharacter::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
// Called to bind functionality to input
//------------------------------------------------------------
void AQLCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    // Set up "action" bindings.
    InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AQLCharacter::StartJump);
    InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AQLCharacter::StopJump);
    InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleCrouch);
    InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleSprint);
    InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AQLCharacter::Fire);
    InputComponent->BindAction("AltFire", EInputEvent::IE_Pressed, this, &AQLCharacter::AltFire);
    InputComponent->BindAction("AltFire", EInputEvent::IE_Released, this, &AQLCharacter::AltFireReleased);
    InputComponent->BindAction("SwitchToGravityGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToGravityGun);
    InputComponent->BindAction("SwitchToPortalGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);
    InputComponent->BindAction("SwitchToNeutronAWP", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToNeutronAWP);
    InputComponent->BindAction("SwitchToLastWeapon", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToLastWeapon);
    InputComponent->BindAction("Test", EInputEvent::IE_Pressed, this, &AQLCharacter::UnlockAllWeapon);
    InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AQLCharacter::ShowInventory);

    // Set up "axis" bindings.
    InputComponent->BindAxis("MoveForward", this, &AQLCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AQLCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AQLCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AQLCharacter::AddControllerPitchInput);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StartJump()
{
    if (0 == DoubleJumpCounter)
    {
        bPressedJump = true;
    }
    else if (1 == DoubleJumpCounter)
    {
        // perform double jump
        LaunchCharacter({0 , 0, GetCharacterMovement()->JumpZVelocity}, false, true);

        PlaySound("DoubleJump");
    }

    ++DoubleJumpCounter;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StopJump()
{
    bPressedJump = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Landed(const FHitResult& Hit)
{
    DoubleJumpCounter = 0;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ToggleCrouch()
{
    // If we are crouching then CanCrouch will return false. If we cannot crouch then calling Crouch() wont do anything
    if (CanCrouch() == true)
    {
        Crouch();
    }
    else
    {
        UnCrouch();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ToggleSprint()
{
    // toggle logic
    bWantToSprint = !bWantToSprint;

    if (CanSprint() == true)
    {
        Sprint();
    }
    else
    {
        UnSprint();
    }
}

//------------------------------------------------------------
// only true when the player wants to sprint and is not sprinting
//------------------------------------------------------------
bool AQLCharacter::CanSprint() const
{
    if (bWantToSprint == true && bIsSprinting == false)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Sprint()
{
    bWantToSprint = true;
    GetCharacterMovement()->MaxWalkSpeed = 3.0f * MaxWalkSpeed;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::UnSprint()
{
    bWantToSprint = false;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::Fire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Fire();
    }
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::AltFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AltFire();
    }
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::AltFireReleased()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AltFireReleased();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToGravityGun()
{
    SwitchToWeapon("GravityGun");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToPortalGun()
{
    SwitchToWeapon("PortalGun");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToNeutronAWP()
{
    SwitchToWeapon("NeutronAWP");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToWeapon(const FName& Name)
{
    // check if the selected weapon is equipped
    if (IsEquipped(Name))
    {
        AQLWeapon* SelectedWeapon = WeaponList[Name];
        // first, unset all other equipped weapon except the selected weapon
        for (auto It = WeaponList.CreateIterator(); It; ++It)
        {
            if (It.Value() && It.Value() != SelectedWeapon)
            {
                It.Value()->ResetWeapon();
            }
        }

        // second, switch to the selected weapon
        ChangeCurrentWeapon(SelectedWeapon);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToLastWeapon()
{
    ChangeCurrentWeapon(LastWeapon);
}

//------------------------------------------------------------
//------------------------------------------------------------
FHitResult AQLCharacter::RayTraceFromCharacterPOV()
{
    FCollisionQueryParams lineTraceParams = FCollisionQueryParams(FName(TEXT("lineTrace")), true, this);
    lineTraceParams.bTraceComplex = true;
    lineTraceParams.bTraceAsyncScene = true;
    lineTraceParams.bReturnPhysicalMaterial = false;

    APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    FVector start = cm->GetCameraLocation();
    FVector end = cm->GetActorForwardVector() * rayTraceRange + start;

    FHitResult hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, lineTraceParams);

    // hit.bBlockingHit  // did ray hit something? (bool)
    // hit.GetActor();   // the hit actor if there is one
    // hit.ImpactPoint;  // FVector
    // hit.ImpactNormal; // FVector

    // for debugging purpose
    // DrawDebugLine(GetWorld(), start, hit.ImpactPoint, FColor(255, 0, 0), true, -1, 0, 10);

    return hit;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IsObjectNextToCharacter(AQLGravityGunCompatibleActor* ggcActor)
{
    if (ggcActor)
    {
        if (GetDistanceTo(ggcActor) <= nextToPlayerThreshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------
// bAllWeaponUnlockable makes sure UnlockAllWeapon()
// can only be called once per character life cycle
//------------------------------------------------------------
void AQLCharacter::UnlockAllWeapon()
{
    if (bAllWeaponUnlockable)
    {
        if (!IsEquipped("GravityGun"))
        {
            PickUpWeapon(GetWorld()->SpawnActor<AQLWeaponGravityGun>(AQLWeaponGravityGun::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!IsEquipped("PortalGun"))
        {
            PickUpWeapon(GetWorld()->SpawnActor<AQLWeaponPortalGun>(AQLWeaponPortalGun::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!CurrentWeapon)
        {
            CurrentWeapon = WeaponList["GravityGun"];
        }

        bAllWeaponUnlockable = false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PickUpWeapon(AQLWeapon* Weapon)
{
    // if the weapon exists
    if (Weapon)
    {
        // if the player does not have this weapon yet
        if (!IsEquipped(Weapon->GetWeaponName()))
        {
            WeaponList[Weapon->GetWeaponName()] = Weapon;
            ChangeCurrentWeapon(Weapon);

            // set logical ownership
            Weapon->SetQLOwner(this);
            AddToInventory(Weapon);

            // physical attachment
            Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

            PlaySound("EquipWeapon");
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeapon* AQLCharacter::GetCurrentWeapon() const
{
    return CurrentWeapon;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::AddToInventory(AQLActor* QLActor)
{
    // if key exists, overwrite the value
    Inventory.Add(QLActor->GetName(), QLActor);
    QLActor->SetQLOwner(this);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::RemoveFromInventory(AQLActor* QLActor)
{
    // if key does not exist, throw a warning
    AActor* Temp;
    bool bKeyExist = Inventory.RemoveAndCopyValue(QLActor->GetName(), Temp);
    QLActor->UnSetQLOwner();

    if (!bKeyExist)
    {
        QLUtility::QLSay(FString("AQLCharacter::RemoveFromInventory(): key does not exist. ") + FString(QLActor->GetName()));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ShowInventory()
{
    for (auto It = Inventory.CreateConstIterator(); It; ++It)
    {
        QLUtility::QLSay(FString("--> character inventory item = ") + It.Value()->GetName() + "     location = " + It.Value()->GetActorLocation().ToString());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ChangeCurrentWeapon(AQLWeapon* Weapon)
{
    // if the target weapon exists
    // note that the current weapon is allowed to be nonexistent
    if (Weapon != nullptr)
    {
        // if the target weapon is not the same with the current weapon
        if (CurrentWeapon != Weapon)
        {
            LastWeapon = CurrentWeapon;
            CurrentWeapon = Weapon;

            PlaySound("SwitchWeapon");
        }
    }
}

//------------------------------------------------------------
// check if the specified type of weapon (identified by name)
// is already equipped by the player
//------------------------------------------------------------
bool AQLCharacter::IsEquipped(const FName& Name)
{
    // if the given name exists in the preset list
    if (WeaponList.Contains(Name))
    {
        // if the player has that type of weapon
        if (WeaponList[Name] != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        QLUtility::QLSay("AQLCharacter::IsEquipped(): unknown weapon type.");
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySound(const FName& SoundName)
{
    QLUtility::PlaySound(SoundList, SoundName, SoundAttenuation);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySound(const FName& SoundName, const FVector& Location)
{
    QLUtility::PlaySound(SoundList, SoundName, Location, SoundAttenuation);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySound2D(const FName& SoundName)
{
    QLUtility::PlaySound(SoundList, SoundName, SoundNoAttenuation);
}

//------------------------------------------------------------
// note: ConstructorHelpers::FObjectFinder<T> and
// CreateDefaultSubobject<T> can only be used inside ctor!!!
//------------------------------------------------------------
UAudioComponent* AQLCharacter::CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> soundWave(soundPath);
    UAudioComponent* soundComp = CreateDefaultSubobject<UAudioComponent>(soundName);

    bool success = false;
    if (soundWave.Object->IsValidLowLevel() && soundComp)
    {
        soundComp->SetSound(soundWave.Object);
        soundComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
        soundComp->SetRelativeLocation(FVector(0.0f));
        soundComp->bAutoActivate = false;
        success = true;
    }

    if (!success)
    {
        QLUtility::QLSay(TEXT("AQLActor::CreateSoundComponent() failed."));
        soundComp = nullptr;
    }

    return soundComp;
}