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

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController::AQLPlayerController()
{
    bGamePaused = false;
    PauseMenuWidget = nullptr;
    PauseMenu = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    FInputActionBinding& PauseToggle = InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AQLPlayerController::Pause);
    PauseToggle.bExecuteWhenPaused = true;
}

//------------------------------------------------------------
// Design requirement:
// To resume the game, either click umg's resume button,
// or press pause key again. umg handles the first case,
// c++ handles the second case.
//
// note that removing umg from viewport does not mean
// release it from memory.
//------------------------------------------------------------
void AQLPlayerController::Pause()
{
    // pause the game
    if (!bGamePaused)
    {
        // only create once in the life cycle of player controller
        if (!PauseMenu)
        {
            // create the pause menu
            PauseMenu = CreateWidget<UQLPauseMenuWidget>(GetWorld(), PauseMenuWidget);
        }

        if (PauseMenu)
        {
            bGamePaused = true;
            bShowMouseCursor = true;
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewport(true);
            SetInputMode(InputMode);
            this->SetPause(true);

            PauseMenu->AddToViewport();
        }
    }
     // resume the game
    else
    {
        // remove the pause menu
        if (PauseMenu)
        {
            bGamePaused = false;
            bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
            this->SetPause(false);

            PauseMenu->RemoveFromViewport();
        }
    }
}