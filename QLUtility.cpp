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
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, string);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName)
    {
        UAudioComponent** valuePtr = SoundList.Find(SoundName);
        if (valuePtr)
        {
            UAudioComponent* soundComp = *valuePtr;

            // does not allow sound pile-up
            // c++ standard: evaluation order from left to right
            //if (soundComp && !soundComp->IsPlaying())

            // allow sound pile-up
            if (soundComp)
            {
                soundComp->Play();
            }
        }
    }
}