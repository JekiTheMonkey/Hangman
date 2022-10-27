#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HM_WordManager.generated.h"

class AHM_WordLetterCell;

USTRUCT()
struct FHM_Word
{
	GENERATED_BODY()

public:
	FString String;

	// All keys must be lowercase
	TMap<TCHAR, TArray<TObjectPtr<AHM_WordLetterCell>>> RemainingLetters;
};

UCLASS()
class HANGMAN_API AHM_WordManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHM_WordManager();

	void RandomNewWord();
	bool IsLetterPresent(FString Letter) const;
	int32 RevealLetter(FString Letter);
	int32 RevealEverything();
	virtual void Reset() override;
	int32 GetRemainingLetters() const { return SelectedWord.RemainingLetters.Num(); }

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	void SpawnCells(FString Word);
	TArray<FVector> ComputeRelativeCellPositions(int32 NumCells) const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Word Manager",
		meta=(AllowPrivateAccess="true"))
	TSubclassOf<AHM_WordLetterCell> WordLetterCellClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Word Manager",
		meta=(AllowPrivateAccess="true"))
	FVector SpacingBetweenCells = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Word Manager",
		meta=(AllowPrivateAccess="true"))
	TArray<FText> WordList;

	FHM_Word SelectedWord;
	
	UPROPERTY()
	TArray<TObjectPtr<AHM_WordLetterCell>> LetterCells;
};
