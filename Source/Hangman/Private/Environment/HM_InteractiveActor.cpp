#include "Environment/HM_InteractiveActor.h"

#include "HM_Tags.h"

AHM_InteractiveActor::AHM_InteractiveActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

bool AHM_InteractiveActor::CanBeInteractedWith(AActor*OtherActor) const
{
	return true;
}

void AHM_InteractiveActor::Interact_Implementation(AActor *OtherActor)
{
	OnInteractDelegate.Broadcast(this, OtherActor, CanBeInteractedWith(OtherActor));
}

void AHM_InteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FHM_Tags::Interactive);
}
