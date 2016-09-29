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
#include "QLGameModeMainMenu.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeMainMenu::AQLGameModeMainMenu()
{
    DefaultPawnClass = AQLCameraPawn::StaticClass();
    PlayerControllerClass = AQLPlayerController::StaticClass();
    HUDClass = AQLHUD::StaticClass();
}