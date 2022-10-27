#include "Character/HM_CharacterMovementComponent.h"

#include "Character/HM_PlayerCharacter.h"

void UHM_CharacterMovementComponent::TickComponent(
	float DeltaSeconds,
	ELevelTick TickType,
	FActorComponentTickFunction *ThisTickFunction)
{
	// Edit max walk speed before the speed computition
	EditMaxWalkSpeed();
	
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);
}

void UHM_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Will be used to edit speed. HmCmc pretends that MaxWalkSpeed does NOT
	// change by any means except itself
	OriginalMaxWalkSpeed = MaxWalkSpeed;

	HmCharacterOwner = CastChecked<AHM_PlayerCharacter>(CharacterOwner);
}

void UHM_CharacterMovementComponent::EditMaxWalkSpeed()
{
	// Don't bother about other movement modes in this project
	if (MovementMode != EMovementMode::MOVE_Walking)
		return;
	
	float Angle = HmCharacterOwner->GetAngleBetweenForwardAndVelocity();
	if (Angle == 0.f)
		return;

	// We don't care about whether it's left or right
	Angle = FMath::Abs(Angle);

	// 0.0 - min deceleration, 1.0 - max delecelation
	const float NormDecel = Angle / 180.f;

	// Example:
	// OrigSpeed 100, MaxSpeedOnOppositeDirectionNormilized 0.5, result 50
	const float MaxDecel = OriginalMaxWalkSpeed * MaxSpeedOnOppositeDirectionNormilized;

	// Compute final speed
	// Example: 100 - 50 * 0.5 (if angle is 90 and the norm value is 0.5)
	MaxWalkSpeed = OriginalMaxWalkSpeed - MaxDecel * NormDecel;
}
