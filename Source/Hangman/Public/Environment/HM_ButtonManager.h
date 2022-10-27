#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HM_ButtonManager.generated.h"

class AHM_InteractiveActor;
class AHM_Button;

USTRUCT(BlueprintType)
struct FHM_ButtonInitParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(ShortTooltip="Must be a single character."))
	FText Letter;
};

USTRUCT(BlueprintType)
struct FHM_ButtonRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FHM_ButtonInitParameters> ButtonParameters;

	UPROPERTY()
	TArray<TObjectPtr<AHM_Button>> SpawnedButtons;
};

UCLASS()
class HANGMAN_API AHM_ButtonManager : public AActor
{
	GENERATED_BODY()

public:
	AHM_ButtonManager();

	virtual void Reset() override;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

	UFUNCTION(BlueprintNativeEvent, Category="HM|Button Manager")
	void OnButtonInteraction(
		AHM_InteractiveActor *ButtonActor, AActor *OtherActor, bool bCanBeInteractedWith);
	virtual void OnButtonInteraction_Implementation(
		AHM_InteractiveActor *ButtonActor, AActor *OtherActor, bool bCanBeInteractedWith);

private:
	void SpawnButtons();
	TArray<TArray<FVector>> ComputeRelativeButtonPositions() const;
	int32 GetMaxNumColumns() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Button Manager",
		meta=(AllowPrivateAccess="true"))
	TSubclassOf<AHM_Button> ButtonClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Button Manager",
		meta=(AllowPrivateAccess="true"))
	FVector SpacingBetweenButtons = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Button Manager",
		meta=(AllowPrivateAccess="true"))
	FVector SpacingBetweenRows = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Button Manager",
		meta=(AllowPrivateAccess="true"))
	TArray<FHM_ButtonRow> ButtonRows;
};
