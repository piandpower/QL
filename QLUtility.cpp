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
    // FIXME: play sound at location crashes using gravity gun may the game
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList,
                   const FName& SoundName,
                   USoundAttenuation* SoundAttenuation)
    {
        if (SoundList.Contains(SoundName))
        {
            UAudioComponent* soundComp = SoundList[SoundName];

            // allow sound pile-up
            if (soundComp)
            {
                // FIXME: PLAY SOUND AT LOCATION WHEN USING GRAVITY GUN SOMETIMES
                // CRASHES THE GAME.
                //QLUtility::QLSay(soundComp->GetComponentLocation().ToString());
                //UGameplayStatics::PlaySoundAtLocation(soundComp->GetWorld(),
                //    soundComp->Sound,
                //    soundComp->GetComponentLocation(),
                //    FRotator::ZeroRotator,
                //    1.0f,
                //    1.0f,
                //    0.0f,
                //    SoundAttenuation);
                UGameplayStatics::PlaySound2D(soundComp->GetWorld(),
                    soundComp->Sound,
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
                // FIXME: PLAY SOUND AT LOCATION WHEN USING GRAVITY GUN SOMETIMES
                // CRASHES THE GAME.
                //UGameplayStatics::PlaySoundAtLocation(soundComp->GetWorld(),
                //    soundComp->Sound,
                //    Location,
                //    FRotator::ZeroRotator,
                //    1.0f,
                //    1.0f,
                //    0.0f,
                //    SoundAttenuation);
                UGameplayStatics::PlaySound2D(soundComp->GetWorld(),
                    soundComp->Sound,
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