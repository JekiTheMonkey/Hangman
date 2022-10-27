#include "Environment/HM_WordManager.h"

#include "HM_Log.h"
#include "Environment/HM_WordLetterCell.h"

AHM_WordManager::AHM_WordManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	WordLetterCellClass = AHM_WordLetterCell::StaticClass();
}

void AHM_WordManager::RandomNewWord()
{
	const int32 RandIndex = FMath::Rand() % (WordList.Num() - 1);
	SelectedWord.String = WordList[RandIndex].ToString();
	SpawnCells(SelectedWord.String);
}

bool AHM_WordManager::IsLetterPresent(FString Letter) const
{
	check(Letter.Len() == 1);

	return !!SelectedWord.RemainingLetters.Find(Letter[0]);
}

int32 AHM_WordManager::RevealLetter(FString Letter)
{
	check(Letter.Len() == 1);

	int32 Revealed = 0;

	const TCHAR LetterCh = FChar::ToLower(Letter[0]);
	auto Found = SelectedWord.RemainingLetters.Find(LetterCh);
	if (!Found)
		return 0;
	
	for (auto &Item : *Found)
	{
		Item->Reveal();
		Revealed++;
	}

	// Letter(s) has(ve) been revealed, hence remove
	SelectedWord.RemainingLetters.Remove(LetterCh);

	return Revealed;
}

int32 AHM_WordManager::RevealEverything()
{
	int32 Revealed = 0;
	for (auto &Letters : SelectedWord.RemainingLetters)
	{
		for (auto &It : Letters.Value)
		{
			It->Reveal();
			Revealed++;
		}
	}

	// Everything has been revealed, hence nothing remains
	SelectedWord.RemainingLetters.Empty();
	return Revealed;
}

void AHM_WordManager::Reset()
{
	Super::Reset();

	// Delete old cells
	for (AHM_WordLetterCell *Cell : LetterCells)
	{
		if (!IsValid(Cell))
			continue;
		Cell->Destroy();
	}
	
	SelectedWord.String.Empty();
	SelectedWord.RemainingLetters.Empty();
	LetterCells.Empty();
	
	RandomNewWord();
}

void AHM_WordManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (!WordLetterCellClass)
	{
		HMS_WARN("WordLetterCellClass is NULL");
		return;
	}

	if (WordList.IsEmpty())
	{
		HMS_WARN("WordList is empty");
		return;
	}

	RandomNewWord();
}

void AHM_WordManager::SpawnCells(FString Word)
{
	check(WordLetterCellClass);

	const int32 Len = Word.Len();
	
	const TArray<FVector> Positions = ComputeRelativeCellPositions(Len);
	const FVector Origin = GetActorLocation();
	const FRotator Rotation = GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	UWorld *World = GetWorld();

	for (int32 Index = 0; Index < Len; Index++)
	{
		const FVector Position = Positions[Index] + Origin;
		auto Cell = Cast<AHM_WordLetterCell>(World->SpawnActor(WordLetterCellClass, &Position, &Rotation, SpawnParameters));

		const TCHAR Letter = Word[Index];

		Cell->SetLetter(Letter);
		LetterCells.Add(Cell);

		// A letter may be already present, hence try to find as well
		auto &Array = SelectedWord.RemainingLetters.FindOrAdd(FChar::ToLower(Letter));
		Array.Add(Cell);
	}
}

TArray<FVector> AHM_WordManager::ComputeRelativeCellPositions(int32 NumCells) const
{
	const FRotator Rotation = GetActorRotation();
	const FVector RotatedSpacing = Rotation.RotateVector(SpacingBetweenCells);
	
	const FVector BoardSize = (NumCells - 1) * RotatedSpacing;
	const FVector HalfSize = BoardSize / 2.f;

	TArray<FVector> Result;
	
	for (int32 Index = 0; Index < NumCells; Index++)
	{
		Result.Add((RotatedSpacing * Index) - (HalfSize));
	}

	return Result;
}
