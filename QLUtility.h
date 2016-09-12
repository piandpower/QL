//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

namespace QLUtility
{
    void QLSay(const FString& string);
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName);
}
