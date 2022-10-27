#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HM_InteractiveActor.generated.h"

class AHM_InteractiveActor;

DECLARE_MULTICAST_DELEGATE_ThreeParams(
	FInteractSignature,
	AHM_InteractiveActor* /* Us */,
	AActor* /* Who is interacting */,
	bool /* Interaction success */);

UCLASS()
class HANGMAN_API AHM_InteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHM_InteractiveActor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="HM|Interactions")
	virtual bool CanBeInteractedWith(AActor *OtherActor) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HM|Interactions")
	void Interact(AActor *OtherActor);
	virtual void Interact_Implementation(AActor *OtherActor);

public:
	FInteractSignature OnInteractDelegate;

protected:
	virtual void BeginPlay() override;
};
