#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "HM_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class HANGMAN_API AHM_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHM_PlayerCharacter();

	//~ Begin APawn Interface.
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	//~ End APawn Interface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="HM|Character")
	float GetAngleBetweenForwardAndVelocity() const;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	UFUNCTION()
	void OnMoveForward(float Ratio);
	
	UFUNCTION()
	void OnMoveRight(float Ratio);

	UFUNCTION()
	void OnLookUp(float Ratio);

	UFUNCTION()
	void OnLookRight(float Ratio);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
		meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> Camera = nullptr;
};
