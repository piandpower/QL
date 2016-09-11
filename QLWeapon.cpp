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

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    FString Name = FString(TEXT(""));
    bIsFireHeldDown = false;
    bIsAltFireHeldDown = false;
    bIsAltFirePressed = false;
    Owner = nullptr;
    CurrentCrosshairTexture = nullptr;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeapon::BeginPlay()
{
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Name + FString(TEXT(" created")));
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeapon::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetWeaponOwner(AQLCharacter* Owner)
{
    this->Owner = Owner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetCurrentCrosshairTexture(const FName& crosshairTextureName)
{
    UTexture2D** valuePtr = CrosshairTextureList.Find(crosshairTextureName);
    if (valuePtr)
    {
        UTexture2D* crosshairTexture = *valuePtr;
        if (crosshairTexture)
        {
            CurrentCrosshairTexture = crosshairTexture;
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UTexture2D* AQLWeapon::GetCurrentCrosshairTexture() const
{
    return CurrentCrosshairTexture;
}

//------------------------------------------------------------
//------------------------------------------------------------
UTexture2D* AQLWeapon::CreateCrosshairTexture(const TCHAR* texturePath)
{
    ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(texturePath);
    UTexture2D* CrosshairTexture = CrosshairTextureObj.Object;

    if(CrosshairTextureObj.Object->IsValidLowLevel() && CrosshairTexture)
    {
        return CrosshairTexture;
    }
    else
    {
        QLUtility::QLSay(TEXT("AQLWeapon::CreateCrosshairTexture() failed."));
        return nullptr;
    }
}

//------------------------------------------------------------
// note: ConstructorHelpers::FObjectFinder<T> and
// CreateDefaultSubobject<T> can only be used inside ctor!!!
//------------------------------------------------------------
UAudioComponent* AQLWeapon::CreateWeaponSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> soundWave(soundPath);
    UAudioComponent* soundComp = CreateDefaultSubobject<UAudioComponent>(soundName);

    bool success = false;
    if (soundWave.Object->IsValidLowLevel() && soundComp)
    {
        soundComp->SetSound(soundWave.Object);
        soundComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        soundComp->SetRelativeLocation(FVector(0.0f));
        soundComp->bAutoActivate = false;
        success = true;
    }

    if (!success)
    {
        QLUtility::QLSay(TEXT("AQLWeapon::CreateWeaponSoundComponent() failed."));
        soundComp = nullptr;
    }

    return soundComp;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayWeaponSound(const FName& soundName)
{
    UAudioComponent** valuePtr = WeaponSoundList.Find(soundName);
    if (valuePtr)
    {
        UAudioComponent* soundComp = *valuePtr;
        // c++ standard: evaluation order from left to right
        if (soundComp && !soundComp->IsPlaying())
        {
            soundComp->Play();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
const FString& AQLWeapon::GetWeaponName() const
{
    return Name;
}