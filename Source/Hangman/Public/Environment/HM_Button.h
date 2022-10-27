#pragma once

#include "CoreMinimal.h"
#include "Environment/HM_InteractiveActor.h"

#include "HM_Button.generated.h"

class UTextRenderComponent;

UENUM()
enum class EHM_ButtonState : uint8
{
	Released,
	PushedSuccess,
	PushedFailed,
	DisplayingError
};

USTRUCT(BlueprintType)
struct FHM_ButtonParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> Material = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor TextColor = FColor::Black;
};

UCLASS()
class HANGMAN_API AHM_Button : public AHM_InteractiveActor
{
	GENERATED_BODY()

public:
	AHM_Button();

	void SetLetter(TCHAR Letter) const;
	TCHAR GetLetter() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="HM|Button")
	FText K2_GetLetter() const;
	
	UFUNCTION(BlueprintCallable, Category="HM|Button")
	void SetPressResult(bool bResult);

	virtual bool CanBeInteractedWith(AActor *OtherActor) const override;
	virtual void Interact_Implementation(AActor *OtherActor) override;

	UFUNCTION(BlueprintCallable, Category="HM|Button")
	virtual void Reset() override;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	void HandleInteractError();
	void SetState(EHM_ButtonState ButtonState);
	void StopDisplayingError();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTextRenderComponent> TextRender = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Button",
		meta=(AllowPrivateAccess="true"))
	TMap<EHM_ButtonState, FHM_ButtonParameters> StateParameters;

	UPROPERTY(BlueprintReadOnly, Category="HM|Button",
		meta=(AllowPrivateAccess="true"))
	EHM_ButtonState State = EHM_ButtonState::Released;
	
	EHM_ButtonState PostDisplayErrorState;

	bool bResultOnPress = true;

	UPROPERTY(EditDefaultsOnly, Category="HM|Button")
	float DisplayErrorSeconds = 1.f;

	FTimerHandle DisplayErrorTimerHandle;
};
