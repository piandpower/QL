// Fill out your copyright notice in the Description page of Project Settings.

#include "QL.h"
#include "QLWeaponPortalGun.h"

AQLWeaponPortalGun::AQLWeaponPortalGun()
{
    Name = "portal gun";
}

void AQLWeaponPortalGun::Fire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("portal gun fire")));
}

void AQLWeaponPortalGun::AltFire()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(TEXT("portal gun alt fire")));
}