// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QLWeaponGravityGun.h"
#include "QLWeaponPortalGun.h"
#include "GameFramework/Character.h"
#include "QLCharacter.generated.h"

// constant
const int maxNumWeapon = 10;

class QL_API QLWeaponManager
{
public:
    QLWeaponManager();
    ~QLWeaponManager();
    AQLWeapon* CurrentWeapon;
    TMap<FString, AQLWeapon*> WeaponList;
    bool IsEquipped(const FString& Name);
};

UCLASS()
class QL_API AQLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* QLCameraComponent;

	AQLCharacter();
    ~AQLCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    virtual void Landed(const FHitResult& Hit) override;

    void RayTrace();

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

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health pickup component")

private:
    QLWeaponManager WeaponManager;
    float RunningTime;
    float FixedInterval;
    int DoubleJumpCounter;

    // bool bHaveSprintAbility; // whether player has gained the ability
    bool bIsSprinting; // whether player is sprinting
    bool bWantToSprint; // player wants to spring

    float MaxWalkSpeed;
    float MaxWalkSpeedCrouched;
};
