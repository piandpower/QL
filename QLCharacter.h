//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "QLUtility.h"
#include "QLWeaponGravityGun.h"
#include "QLWeaponPortalGun.h"
#include "GameFramework/Character.h"
#include "QLCharacter.generated.h"

// constant
const int maxNumWeapon = 10;
const float rayTraceRange = 10000.0f;
const float nextToPlayerThreshold = 400.0f;

class QL_API QLWeaponManager
{
public:
    AQLWeapon* CurrentWeapon;
    AQLWeapon* LastWeapon;

    QLWeaponManager() {};
    QLWeaponManager(AQLCharacter* QLCharacter);
    ~QLWeaponManager();
    TMap<FName, AQLWeapon*> WeaponList;
    bool IsEquipped(const FName& Name);
    void ChangeCurrentWeapon(AQLWeapon* Weapon);

protected:
    AQLCharacter* QLCharacter;
};

UCLASS()
class QL_API AQLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    QLWeaponManager WeaponManager;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* QLCameraComponent;
    UPhysicsHandleComponent* PhysicsHandle;

	AQLCharacter();
    ~AQLCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    virtual void Landed(const FHitResult& Hit) override;

    FHitResult RayTraceFromCharacterPOV();
    bool IsObjectNextToCharacter(AQLGravityGunCompatibleActor* ggcActor);

    // implementation makes sure this function can only be called once
    void UnlockAllWeapon();

    // Handles input for moving forward and backward.
    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    // Sets jump flag when key is pressed.
    UFUNCTION()
    void StartJump();

    // Clears jump flag when key is released.
    UFUNCTION()
    void StopJump();

    // Clears jump flag when key is released.
    UFUNCTION()
    void ToggleCrouch();

    UFUNCTION()
    void ToggleSprint();

    UFUNCTION()
    bool CanSprint() const;

    UFUNCTION()
    void Sprint();

    UFUNCTION()
    void UnSprint();

    UFUNCTION()
    void Fire();

    UFUNCTION()
    void AltFire();

    UFUNCTION()
    void AltFireReleased();

    UFUNCTION()
    void SwitchToGravityGun();

    UFUNCTION()
    void SwitchToPortalGun();

    UFUNCTION()
    void SwitchToNeutronAWP();

    UFUNCTION()
    void SwitchToLastWeapon();

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health pickup component")

    void PickUpWeapon(AQLWeapon* Weapon);

protected:
    int DoubleJumpCounter;

    // bool bHaveSprintAbility; // whether player has gained the ability
    bool bIsSprinting; // whether player is sprinting
    bool bWantToSprint; // player wants to spring

    float MaxWalkSpeed;
    float MaxWalkSpeedCrouched;

    int Health;

    bool bAllWeaponUnlockable;
};
