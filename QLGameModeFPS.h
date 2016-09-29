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

#include "QLGameModeBase.h"
#include "QLGameModeFPS.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLGameModeFPS : public AQLGameModeBase
{
    GENERATED_BODY()

public:
    AQLGameModeFPS();
    virtual void StartPlay() override;
};
