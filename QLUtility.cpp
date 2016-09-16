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
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName)
    {
        PlaySound2D(SoundList, SoundName);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName, const FAttenuationSettings& AttenuationSettings)
    {
        if (SoundList.Contains(SoundName))
        {
            UAudioComponent* soundComp = SoundList[SoundName];

            // does not allow sound pile-up
            // c++ standard: evaluation order from left to right
            //if (soundComp && !soundComp->IsPlaying())

            // allow sound pile-up
            if (soundComp)
            {
                soundComp->AdjustAttenuation(AttenuationSettings);
                soundComp->Play();
            }
        }
        else
        {
            QLUtility::QLSay("sound not found.");
        }
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList,
                   const FName& SoundName,
                   const FVector& Location,
                   USoundAttenuation* SoundAttenuation)
    {
        if (SoundList.Contains(SoundName))
        {
            UAudioComponent* soundComp = SoundList[SoundName];

            // allow sound pile-up
            if (soundComp)
            {
                UGameplayStatics::PlaySoundAtLocation(soundComp->GetWorld(),
                    soundComp->Sound,
                    Location,
                    FRotator::ZeroRotator,
                    1.0f,
                    1.0f,
                    0.0f,
                    SoundAttenuation);
            }
        }
        else
        {
            QLUtility::QLSay("sound not found.");
        }
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySound2D(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName)
    {
        FAttenuationSettings AttenuationSettings;
        AttenuationSettings.bAttenuate = false;
        PlaySound(SoundList, SoundName, AttenuationSettings);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySoundAttenuated(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName)
    {
        FAttenuationSettings AttenuationSettings;
        AttenuationSettings.bAttenuate = true;
        AttenuationSettings.DistanceAlgorithm = ESoundDistanceModel::ATTENUATION_Logarithmic;
        PlaySound(SoundList, SoundName, AttenuationSettings);
    }
}