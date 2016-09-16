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
#include "QLActor.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLActor::AQLActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLOwner = nullptr;
    SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    SoundNoAttenuation->Attenuation.bAttenuate = false;

    SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    SoundAttenuation->Attenuation.bAttenuate = true;
    SoundAttenuation->Attenuation.DistanceAlgorithm = ESoundDistanceModel::ATTENUATION_Linear;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::SetQLOwner(AActor* QLOwner)
{
    this->QLOwner = QLOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::UnSetQLOwner()
{
    this->QLOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AActor* AQLActor::GetQLOwner()
{
    return QLOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::PlaySound(const FName& soundName)
{
    QLUtility::PlaySound(SoundList, soundName);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::PlaySound(const FName& soundName, const FVector& Location)
{
    QLUtility::PlaySound(SoundList, soundName, Location, SoundAttenuation);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::PlaySound2D(const FName& soundName)
{
    QLUtility::PlaySound2D(SoundList, soundName);
}

//------------------------------------------------------------
// note: ConstructorHelpers::FObjectFinder<T> and
// CreateDefaultSubobject<T> can only be used inside ctor!!!
//------------------------------------------------------------
UAudioComponent* AQLActor::CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName)
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