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
#include "QLWeapon.h"

// Sets default values
AQLWeapon::AQLWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    FString Name = FString(TEXT(""));
    bIsFireHeldDown = false;
    bIsAltFireHeldDown = false;
    owner = nullptr;
    CrosshairTexture = nullptr;
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

UTexture2D* AQLWeapon::GetCrosshairTexture() const
{
    return CrosshairTexture;
}

void AQLWeapon::SetCrosshairTexture(const TCHAR* textureName)
{
    ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(textureName);
    CrosshairTexture = CrosshairTextureObj.Object;
}

const FString& AQLWeapon::GetWeaponName() const
{
    return Name;
}