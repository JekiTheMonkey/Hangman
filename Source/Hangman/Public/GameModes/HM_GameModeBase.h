#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "HM_GameModeBase.generated.h"

class AHM_ButtonManager;
class AHM_WordManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicMulticastSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameOverSignature, bool, bWon);

UENUM(BlueprintType)
enum class EHM_GameState : uint8
{
	InProgress,
	Won,
	Lost
};

UCLASS()
class HANGMAN_API AHM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHM_GameModeBase();

	UFUNCTION(BlueprintCallable, Category="HM|Game Mode Base")
	int32 TryRevealLetter(FString Letter);

	UFUNCTION(BlueprintCallable, Category="HM|Game Mode Base")
	bool DecreaseTries();

	int32 GetRemainingTries() const { return RemainingTries; }

	EHM_GameState GetGameState() const { return HmGameState; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HM|Game Mode Base")
	void Restart();
	virtual void Restart_Implementation();

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

	UFUNCTION(BlueprintNativeEvent, Category="HM|Game Mode Base")
	void OnGameOver(bool bSuccess);
	virtual void OnGameOver_Implementation(bool bSuccess);

private:
	void FinishGame(bool bSuccess);
	
	AHM_ButtonManager *FindButtonManager() const;
	AHM_WordManager *FindWordManager() const;
	
public:
	UPROPERTY(BlueprintAssignable)
	FGameOverSignature GameOverDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FDynamicMulticastSignature RestartDelegate;

	UPROPERTY(BlueprintAssignable)
	FDynamicMulticastSignature OnTryFailedDelegate;

private:
	UPROPERTY(BlueprintReadOnly, Category="HM|Game Mode Base",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<AHM_ButtonManager> ButtonManagerTarget = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category="HM|Game Mode Base",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<AHM_WordManager> WordManagerTarget = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Game Mode Base",
		meta=(AllowPrivateAccess="true"))
	int32 MaxTries = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HM|Game Mode Base|Runtime",
		meta=(AllowPrivateAccess="true"))
	int32 RemainingTries = 0;

	UPROPERTY(BlueprintReadOnly, Category="HM|Game Mode Base",
		meta=(AllowPrivateAccess="true"))
	EHM_GameState HmGameState = EHM_GameState::InProgress;
};
