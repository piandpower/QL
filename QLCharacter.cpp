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

QLWeaponManager::QLWeaponManager(AQLCharacter* QLCharacter)
{
    this->QLCharacter = QLCharacter;
    WeaponList.Add("GravityGun", nullptr);
    WeaponList.Add("PortalGun",  nullptr);
    WeaponList.Add("NeutronAWP", nullptr);
    CurrentWeapon = nullptr;
    LastWeapon = nullptr;
}

QLWeaponManager::~QLWeaponManager()
{
    // iterate all weapons the player possesses
    for (auto it = WeaponList.CreateIterator(); it; ++it)
    {
        if (IsEquipped(it.Key()))
        {
            it.Value()->Destroy();
        }
    }
}

void QLWeaponManager::ChangeCurrentWeapon(AQLWeapon* Weapon)
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
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("switch to ")) + Weapon->GetWeaponName());
        }
    }
}

void QLWeaponManager::PickUpWeapon(AQLWeapon* Weapon)
{
    // if the weapon exists
    if (Weapon)
    {
        ChangeCurrentWeapon(Weapon);
        Weapon->SetOwner(QLCharacter);
    }
}

// check if the specified weapon is equipped by the player
bool QLWeaponManager::IsEquipped(const FString& Name)
{
    if (WeaponList[Name] != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Sets default values
AQLCharacter::AQLCharacter() : WeaponManager(this)
{
    RunningTime = 0.0f;
    FixedInterval = 1.0f;
    DoubleJumpCounter = 0;

    bIsSprinting = false;
    bWantToSprint = false;

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // movement
    GetCharacterMovement()->JumpZVelocity = 400.0f;
    GetCharacterMovement()->AirControl = 1.0f;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
    MaxWalkSpeedCrouched = 100.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;

    // camera
    QLCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    QLCameraComponent->AttachTo(GetCapsuleComponent());
    QLCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f + BaseEyeHeight));
    QLCameraComponent->bUsePawnControlRotation = true;
}

AQLCharacter::~AQLCharacter()
{
}

// Called when the game starts or when spawned
void AQLCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AQLCharacter::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    //// display message every fixed interval
    //RunningTime += DeltaTime;
    //if(RunningTime > FixedInterval)
    //{
    //    if (GEngine)
    //    {
    //        // Display a debug message for five seconds.
    //        // The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
    //        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("player speed = ") + FString::SanitizeFloat(GetVelocity().Size()));
    //        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, GetCharacterMovement()->GetMovementName());
    //    }

    //    RunningTime -= FixedInterval;
    //}
}

// Called to bind functionality to input
void AQLCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    // Set up "action" bindings.
    InputComponent->BindAction("Jump", IE_Pressed, this, &AQLCharacter::StartJump);
    InputComponent->BindAction("Jump", IE_Released, this, &AQLCharacter::StopJump);
    InputComponent->BindAction("Crouch", IE_Pressed, this, &AQLCharacter::ToggleCrouch);
    InputComponent->BindAction("Sprint", IE_Pressed, this, &AQLCharacter::ToggleSprint);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AQLCharacter::Fire);
    InputComponent->BindAction("AltFire", IE_Pressed, this, &AQLCharacter::AltFire);
    InputComponent->BindAction("AltFire", IE_Released, this, &AQLCharacter::AltFireReleased);
    InputComponent->BindAction("SwitchToGravityGun", IE_Pressed, this, &AQLCharacter::SwitchToGravityGun);
    InputComponent->BindAction("SwitchToPortalGun", IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);
    InputComponent->BindAction("SwitchToNeutronAWP", IE_Pressed, this, &AQLCharacter::SwitchToNeutronAWP);
    InputComponent->BindAction("SwitchToLastWeapon", IE_Pressed, this, &AQLCharacter::SwitchToLastWeapon);
    InputComponent->BindAction("Test", IE_Pressed, this, &AQLCharacter::UnlockAllWeapon);

    // Set up "axis" bindings.
    InputComponent->BindAxis("MoveForward", this, &AQLCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AQLCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AQLCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AQLCharacter::AddControllerPitchInput);
}

void AQLCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AQLCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AQLCharacter::StartJump()
{
    if (0 == DoubleJumpCounter)
    {
        bPressedJump = true;
    }
    else if (1 == DoubleJumpCounter)
    {
        LaunchCharacter({0 , 0, 2.0f * GetCharacterMovement()->JumpZVelocity}, false, true);
    }

    ++DoubleJumpCounter;
}

void AQLCharacter::StopJump()
{
    bPressedJump = false;
}

void AQLCharacter::Landed(const FHitResult& Hit)
{
    DoubleJumpCounter = 0;
}

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

// only true when the player wants to sprint and is not sprinting
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

void AQLCharacter::Sprint()
{
    bWantToSprint = true;
    GetCharacterMovement()->MaxWalkSpeed = 2.0f * MaxWalkSpeed;
}

void AQLCharacter::UnSprint()
{
    bWantToSprint = false;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

// delegate to weapon member function
void AQLCharacter::Fire()
{
    if (WeaponManager.CurrentWeapon)
    {
        WeaponManager.CurrentWeapon->Fire();
    }
}

// delegate to weapon member function
void AQLCharacter::AltFire()
{
    if (WeaponManager.CurrentWeapon)
    {
        WeaponManager.CurrentWeapon->AltFire();
    }
}

// delegate to weapon member function
void AQLCharacter::AltFireReleased()
{
    if (WeaponManager.CurrentWeapon)
    {
        WeaponManager.CurrentWeapon->AltFireReleased();
    }
}

void AQLCharacter::SwitchToGravityGun()
{
    if (WeaponManager.IsEquipped(FString(TEXT("GravityGun"))))
    {
        WeaponManager.ChangeCurrentWeapon(WeaponManager.WeaponList["GravityGun"]);
    }
}

void AQLCharacter::SwitchToPortalGun()
{
    if (WeaponManager.IsEquipped(FString(TEXT("PortalGun"))))
    {
        WeaponManager.ChangeCurrentWeapon(WeaponManager.WeaponList["PortalGun"]);
    }
}

void AQLCharacter::SwitchToNeutronAWP()
{
    if (WeaponManager.IsEquipped(FString(TEXT("NeutronAWP"))))
    {
        WeaponManager.CurrentWeapon = WeaponManager.WeaponList["NeutronAWP"];
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("try to switch to neutron awp.")));
}

void AQLCharacter::SwitchToLastWeapon()
{
    WeaponManager.ChangeCurrentWeapon(WeaponManager.LastWeapon);
}

void AQLCharacter::RayTrace()
{
    FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
    RV_TraceParams.bTraceComplex = true;
    RV_TraceParams.bTraceAsyncScene = true;
    RV_TraceParams.bReturnPhysicalMaterial = false;

    APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    FVector start = cm->GetCameraLocation();
    FVector end = cm->GetActorForwardVector() * rayTraceEnd + start;

    FHitResult RV_Hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(RV_Hit, start, end, ECC_Pawn, RV_TraceParams);

    // RV_Hit.bBlockingHit //did hit something? (bool)
    // RV_Hit.GetActor(); //the hit actor if there is one
    // RV_Hit.ImpactPoint;  //FVector
    // RV_Hit.ImpactNormal;  //FVector
    FString bBlockingHitString = RV_Hit.bBlockingHit ? TEXT("true") : TEXT("false");
    FString ImpactPointFVertor = RV_Hit.ImpactPoint.ToString();
    DrawDebugLine(GetWorld(), start, RV_Hit.ImpactPoint, FColor(255, 0, 0), true, -1, 0, 10);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, bBlockingHitString + FString(TEXT("  ")) + ImpactPointFVertor);
}

void AQLCharacter::UnlockAllWeapon()
{
    WeaponManager.WeaponList["GravityGun"] = GetWorld()->SpawnActor<AQLWeaponGravityGun>(AQLWeaponGravityGun::StaticClass());
    WeaponManager.WeaponList["GravityGun"]->SetOwner(this);

    WeaponManager.WeaponList["PortalGun"]  = GetWorld()->SpawnActor<AQLWeaponPortalGun>(AQLWeaponPortalGun::StaticClass());
    WeaponManager.WeaponList["PortalGun"]->SetOwner(this);

    //WeaponManager.WeaponList["NeutronAWP"] = GetWorld()->SpawnActor<AQLWeaponNeutronAWP>(AQLWeaponPortalGun::StaticClass());
    //WeaponManager.WeaponList["NeutronAWP"]->SetOwner(this);

    WeaponManager.CurrentWeapon = WeaponManager.WeaponList["GravityGun"];
}