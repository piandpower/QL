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

#include "QLUtility.h"
#include "Blueprint/UserWidget.h"
#include "QLPlayGameMenuWidget.generated.h"

UCLASS()
class QL_API UQLPlayGameMenuWidget : public UUserWidget
{
public:
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void PlayGame();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetPlayerController(AQLPlayerController* PlayerController);

private:
    AQLPlayerController* PlayerController;
};
