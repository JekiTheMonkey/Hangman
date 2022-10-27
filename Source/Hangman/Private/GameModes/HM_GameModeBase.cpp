#include "GameModes/HM_GameModeBase.h"

#include "HM_Log.h"
#include "Character/HM_PlayerCharacter.h"
#include "Character/HM_PlayerController.h"
#include "Environment/HM_ButtonManager.h"
#include "Environment/HM_WordManager.h"
#include "Kismet/GameplayStatics.h"

AHM_GameModeBase::AHM_GameModeBase()
{
	DefaultPawnClass = AHM_PlayerCharacter::StaticClass();
	PlayerControllerClass = AHM_PlayerController::StaticClass();
}

int32 AHM_GameModeBase::TryRevealLetter(FString Letter)
{
	check(Letter.Len() == 1);
	
	if (!IsValid(WordManagerTarget))
		return 0;

	const int32 Num = WordManagerTarget->RevealLetter(Letter);
	if (Num == 0)
	{
		DecreaseTries();
	}
	else if (WordManagerTarget->GetRemainingLetters() == 0)
	{
		FinishGame(true);
	}
	
	return Num;
}

bool AHM_GameModeBase::DecreaseTries()
{
	if (RemainingTries == 0)
		return true;
	
	RemainingTries = RemainingTries - 1;
	OnTryFailedDelegate.Broadcast();
	
	// Ran out of tries
	if (HmGameState == EHM_GameState::InProgress && RemainingTries == 0)
	{
		FinishGame(false);
	}
	
	return HmGameState == EHM_GameState::InProgress;
}

void AHM_GameModeBase::Restart_Implementation()
{
	RemainingTries = MaxTries;
	HmGameState = EHM_GameState::InProgress;

	WordManagerTarget->Reset();
	ButtonManagerTarget->Reset();

	RestartDelegate.Broadcast();
}

void AHM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RemainingTries = MaxTries;

	ButtonManagerTarget = FindButtonManager();
	WordManagerTarget = FindWordManager();
}

AHM_ButtonManager *AHM_GameModeBase::FindButtonManager() const
{
	TArray<AActor*> Actors;
	
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), AHM_ButtonManager::StaticClass(), Actors);
	const int32 NumButtonManagers = Actors.Num();
	
	if (NumButtonManagers == 0)
	{
		HMS_WARN("No AHM_ButtonManager found on the scene");
		return nullptr;
	}
	if (NumButtonManagers > 1)
	{
		HMS_WARN("Found more then one AHM_ButtonManager. Only the first "
			"instance [%s] will be used", *Actors[0]->GetName());
	}

	return Cast<AHM_ButtonManager>(Actors[0]);
}

AHM_WordManager *AHM_GameModeBase::FindWordManager() const
{
	TArray<AActor*> Actors;
	
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), AHM_WordManager::StaticClass(), Actors);
	const int32 NumWordManagers = Actors.Num();
	
	if (NumWordManagers == 0)
	{
		HMS_WARN("No AHM_WordManager found on the scene");
		return nullptr;
	}
	if (NumWordManagers > 1)
	{
		HMS_WARN("Found more then one AHM_WordManager. Only the first "
			"instance [%s] will be used", *Actors[0]->GetName());
	}

	return Cast<AHM_WordManager>(Actors[0]);
}

void AHM_GameModeBase::OnGameOver_Implementation(bool bSuccess)
{
	if (!IsValid(WordManagerTarget))
		return;

	WordManagerTarget->RevealEverything();
	HMS_LOG("Game is over. Success: %d", bSuccess);
}

void AHM_GameModeBase::FinishGame(bool bSuccess)
{
	HmGameState = bSuccess ? EHM_GameState::Won : EHM_GameState::Lost;
	GameOverDelegate.Broadcast(bSuccess);
}

