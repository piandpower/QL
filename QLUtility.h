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
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName);
    void PlaySound2D(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName);
    void PlaySoundAttenuated(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName);
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList, const FName& SoundName, const FAttenuationSettings& AttenuationSettings);
    void PlaySound(TMap<FName, UAudioComponent*>& SoundList,
                   const FName& SoundName,
                   const FVector& Location,
                   USoundAttenuation* SoundAttenuation);
}
