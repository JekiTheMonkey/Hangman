#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HM_WordLetterCell.generated.h"

class UTextRenderComponent;

UCLASS()
class HANGMAN_API AHM_WordLetterCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AHM_WordLetterCell();

	void SetLetter(TCHAR Letter) const;
	TCHAR GetLetter() const;

	UFUNCTION(BlueprintNativeEvent, Category="HM|Word Letter Cell")
	void Reveal();
	virtual void Reveal_Implementation();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTextRenderComponent> NoLetterTextRender = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTextRenderComponent> LetterTextRender = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HM|Word Letter Cell",
		meta=(AllowPrivateAccess="true"))
	FText NoLetterText;
};
