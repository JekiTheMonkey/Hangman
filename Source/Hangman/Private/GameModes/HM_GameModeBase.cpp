#include "GameModes/HM_GameModeBase.h"

#include "Character/HM_PlayerCharacter.h"
#include "Character/HM_PlayerController.h"

AHM_GameModeBase::AHM_GameModeBase()
{
	DefaultPawnClass = AHM_PlayerCharacter::StaticClass();
	PlayerControllerClass = AHM_PlayerController::StaticClass();
}
