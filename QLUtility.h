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
    void QLSayLong(const FString& string);
    void QLSay(const FString& string, const float time);

    //------------------------------------------------------------
    // play sound
    // (1) at component's location
    // (2) with given attenuation setting
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList,
                   const FName& SoundName,
                   const FAttenuationSettings& AttenuationSettings);

    //------------------------------------------------------------
    // play sound
    // (1) at given location
    // (2) with given attenuation setting
    //------------------------------------------------------------
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList,
                   const FName& SoundName,
                   const FVector& Location,
                   USoundAttenuation* SoundAttenuation);
}
