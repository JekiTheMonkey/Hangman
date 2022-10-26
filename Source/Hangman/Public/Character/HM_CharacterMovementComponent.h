#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "HM_CharacterMovementComponent.generated.h"

class AHM_PlayerCharacter;

UCLASS()
class HANGMAN_API UHM_CharacterMovementComponent : 
	public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(
		float DeltaSeconds,
		ELevelTick TickType, 
		FActorComponentTickFunction *ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void EditMaxWalkSpeed();

private:
	UPROPERTY()
	TObjectPtr<AHM_PlayerCharacter> HmCharacterOwner = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking",
		meta=(ClampMin="0.0", ClampMax="1.0"))
	float MaxSpeedOnOppositeDirectionNormilized = 0.5f;
	
	float OriginalMaxWalkSpeed = 0.f;
};
