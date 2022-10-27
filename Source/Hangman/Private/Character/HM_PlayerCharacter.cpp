#include "Character/HM_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AHM_PlayerCharacter::AHM_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	CameraBoom->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;

	UCharacterMovementComponent *Cmc = GetCharacterMovement();
	Cmc->bUseControllerDesiredRotation = false;
	Cmc->bOrientRotationToMovement = true;
	Cmc->bIgnoreBaseRotation = false;
	Cmc->RotationRate.Roll = 180.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AHM_PlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::OnMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::OnMoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ThisClass::OnLookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ThisClass::OnLookUp);
}

// Note: Can be cached since the function is called from two sources each frame
float AHM_PlayerCharacter::GetAngleBetweenForwardAndVelocity() const
{
	FVector Velocity = GetVelocity();
	if (Velocity.IsZero())
		return 0.f;

	const FVector Forward = GetActorForwardVector();
	Velocity = Velocity.GetSafeNormal();
	
	const float Dot = FVector::DotProduct(Velocity, Forward);
	const FVector Cross(FVector::CrossProduct(Velocity, Forward));

	const float AcosDeg = FMath::RadiansToDegrees(FMath::Acos(Dot));
	
	return Cross.IsZero() ? (AcosDeg) : (AcosDeg * FMath::Sign(Cross.Z));
}

void AHM_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

static FVector GetDirection(FRotator Rotation, EAxis::Type Axis)
{
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	return FRotationMatrix(YawRotation).GetUnitAxis(Axis);
}

void AHM_PlayerCharacter::OnMoveForward(float Ratio)
{
	if (Ratio == 0.f || !IsValid(Controller))
		return;

	const FVector Direction = GetDirection(Controller->GetControlRotation(), EAxis::X);

	AddMovementInput(Direction, Ratio);
}

void AHM_PlayerCharacter::OnMoveRight(float Ratio)
{
	if (Ratio == 0.f || !IsValid(Controller))
		return;

	const FVector Direction = GetDirection(Controller->GetControlRotation(), EAxis::Y);

	AddMovementInput(Direction, Ratio);
}

void AHM_PlayerCharacter::OnLookUp(float Ratio)
{
	if (Ratio == 0.f)
		return;

	AddControllerPitchInput(Ratio);
}

void AHM_PlayerCharacter::OnLookRight(float Ratio)
{
	if (Ratio == 0.f)
		return;
	
	AddControllerYawInput(Ratio);
}
