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
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Name = "None";
    bIsFireHeldDown = false;
    bIsAltFireHeldDown = false;
    bIsAltFirePressed = false;
    Owner = nullptr;
    CurrentCrosshairTexture = nullptr;

    UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    BoxComponent->InitBoxExtent(FVector(10.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    RootComponent = BoxComponent;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLWeapon::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeapon::BeginPlay()
{
    Super::BeginPlay();
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
    // set logical ownership
    // so that the weapon will know which character is owning it
    // and can call character's member function
    this->Owner = Owner;

    // set logical ownership
    Owner->AddToInventory(this);
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
    QLUtility::PlaySound(WeaponSoundList, soundName);
}

//------------------------------------------------------------
//------------------------------------------------------------
const FName& AQLWeapon::GetWeaponName() const
{
    return Name;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    AQLCharacter* player = Cast<AQLCharacter>(OtherActor);
    // if the overlapping actor is player, equip him with the weapon
    if (player)
    {
        player->PickUpWeapon(this);
    }
}