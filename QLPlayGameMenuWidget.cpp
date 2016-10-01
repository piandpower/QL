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
#include "QLPlayerController.h"
#include "QLPlayGameMenuWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayGameMenuWidget::PlayGame()
{
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->SetPause(false);

        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("QLArena")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayGameMenuWidget::SetPlayerController(AQLPlayerController* PlayerController)
{
    this->PlayerController = PlayerController;
}