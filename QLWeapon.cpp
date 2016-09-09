// Fill out your copyright notice in the Description page of Project Settings.

#include "QL.h"
#include "QLWeapon.h"

// Sets default values
AQLWeapon::AQLWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bIsFireHeldDown = false;
    bIsAltFireHeldDown = false;
}

// Called when the game starts or when spawned
void AQLWeapon::BeginPlay()
{
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Name + FString(TEXT(" created")));
}

// Called every frame
void AQLWeapon::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

void AQLWeapon::SetOwner(AQLCharacter* owner)
{
    this->owner = owner;
}