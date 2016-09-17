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
#include "QLUtility.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSay(const FString& string)
    {
        QLSay(string, 5.0f);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSayLong(const FString& string)
    {
        QLSay(string, 600.0f);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSay(const FString& string, const float time)
    {
        GEngine->AddOnScreenDebugMessage(-1, time, FColor::Cyan, string);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySoundComponent(TMap<FName, UAudioComponent*>& SoundComponentList, const FName& SoundName)
    {
        if (SoundComponentList.Contains(SoundName))
        {
            UAudioComponent* soundComp = SoundComponentList[SoundName];

            // allow sound pile-up
            if (soundComp)
            {
                soundComp->Play();
            }
        }
        else
        {
            QLUtility::QLSay("sound not found.");
        }
    }

    //------------------------------------------------------------
    // FIXME: play sound at location using gravity gun may crash the game
    //------------------------------------------------------------
    void PlaySoundFireAndForget(UObject* World,
                                TMap<FName, USoundWave*>& SoundWaveList,
                                const FName& SoundName,
                                const FVector& Location,
                                USoundAttenuation* SoundAttenuation)
    {
        if (SoundWaveList.Contains(SoundName))
        {
            USoundWave* SoundWave = SoundWaveList[SoundName];

            // allow sound pile-up
            if (SoundWave)
            {
                // FIXME: ATTENUATION CAUSES THE GAME TO CRASH RANDOMLY
                //UGameplayStatics::PlaySoundAtLocation(World,
                //    SoundWave,
                //    Location,
                //    FRotator::ZeroRotator,
                //    1.0f,
                //    1.0f,
                //    0.0f,
                //    SoundAttenuation);

                UGameplayStatics::PlaySound2D(World,
                    SoundWave,
                    1.0f,
                    1.0f,
                    0.0f);
            }
        }
        else
        {
            QLUtility::QLSay("sound not found.");
        }
    }
}