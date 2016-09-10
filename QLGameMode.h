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

#include "GameFramework/GameMode.h"
#include "QLGameMode.generated.h"

UCLASS()
class QL_API AQLGameMode : public AGameMode
{
	GENERATED_BODY()
    virtual void StartPlay() override;
};
