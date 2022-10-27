#include "Environment/HM_ButtonManager.h"

#include "HM_Log.h"
#include "Environment/HM_Button.h"

AHM_ButtonManager::AHM_ButtonManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	ButtonClass = AHM_Button::StaticClass();
}

void AHM_ButtonManager::Reset()
{
	Super::Reset();

	for (FHM_ButtonRow &Row : ButtonRows)
	{
		for (AHM_Button *Button : Row.SpawnedButtons)
			Button->Reset();
	}
}

void AHM_ButtonManager::BeginPlay()
{
	Super::BeginPlay();

	if (!ButtonClass)
	{
		HMS_WARN("Button class is NULL");
		return;
	}

	SpawnButtons();
}

void AHM_ButtonManager::OnButtonInteraction_Implementation(
	AHM_InteractiveActor *ButtonActor, AActor *OtherActor, bool bCanBeInteractedWith)
{
}

void AHM_ButtonManager::SpawnButtons()
{
	check(ButtonClass);

	if (ButtonRows.IsEmpty())
		return;

	UWorld *World = GetWorld();
	const FVector Origin = GetActorLocation();
	TArray<TArray<FVector>> ButtonPositions = ComputeRelativeButtonPositions();
	const FRotator Rotation = GetActorRotation();
	
	int32 RowIndex = 0;
	for (FHM_ButtonRow &Row : ButtonRows)
	{
		int32 ColumnIndex = 0;
		for (const FHM_ButtonInitParameters &ButtonParams : Row.ButtonParameters)
		{
			const FString LetterStr = ButtonParams.Letter.ToString();
			if (LetterStr.Len() != 1)
			{
				HMS_WARN("Button[%d][%d] has an invalid letter", RowIndex, ColumnIndex);
				ColumnIndex++;
				continue;
			}
			
			// Define spawn parameters
			const FVector RotatedLocation = ButtonPositions[RowIndex][ColumnIndex];
			const FVector Location = Origin + RotatedLocation;
			
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn, setup, save the button
			auto Button = Cast<AHM_Button>(World->SpawnActor(ButtonClass, &Location, &Rotation, SpawnParameters));
			
			Button->OnInteractDelegate.AddUObject(this, &ThisClass::OnButtonInteraction);
			Button->SetLetter(LetterStr[0]);
			
			Row.SpawnedButtons.Add(Button);

			ColumnIndex++;
		}

		RowIndex++;
	}
}

TArray<TArray<FVector>> AHM_ButtonManager::ComputeRelativeButtonPositions() const
{
	check(ButtonClass);

	TArray<TArray<FVector>> Result;
	if (ButtonRows.IsEmpty())
		return Result;

	const int32 Rows = ButtonRows.Num();
	const int32 Columns = GetMaxNumColumns();

	const FRotator Rotation = GetActorRotation();
	const FVector RotatedButtonsSpacing = Rotation.RotateVector(SpacingBetweenButtons);
	const FVector RotatedRowsSpacing = Rotation.RotateVector(SpacingBetweenRows);
	
	const FVector TableSize = (RotatedButtonsSpacing * (Columns - 1)) + (RotatedRowsSpacing * (Rows - 1));
	const FVector HalfTableSize = TableSize / 2.f;

	int32 RowIndex = 0;
	for (const FHM_ButtonRow &Row : ButtonRows)
	{
		Result.Add({});
		const int32 Num = Row.ButtonParameters.Num();
		for (int32 ColumnIndex = 0; ColumnIndex < Num; ColumnIndex++)
		{
			const FVector Position(
				(RotatedButtonsSpacing * ColumnIndex) +
				(RotatedRowsSpacing * RowIndex) -
				(HalfTableSize));

			Result.Last().Add(Position);
		}
		
		RowIndex++;
	}

	return Result;
}

int32 AHM_ButtonManager::GetMaxNumColumns() const
{
	int32 Max = 0;
	for (const FHM_ButtonRow &Row : ButtonRows)
	{
		const int32 Num = Row.ButtonParameters.Num();
		if (Max < Num)
			Max = Num;
	}
	return Max;
}
