#include "Environment/HM_Button.h"

#include "HM_Log.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/HM_GameModeBase.h"

AHM_Button::AHM_Button()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Renderer Component"));
	TextRender->SetupAttachment(StaticMesh);
}

void AHM_Button::SetLetter(TCHAR Letter) const
{
	FString LetterStr;
	LetterStr.AppendChar(Letter);
	
	TextRender->SetText(FText::FromString(LetterStr));
}

TCHAR AHM_Button::GetLetter() const
{
	return TextRender->Text.IsEmpty() ? 'a' : TextRender->Text.ToString()[0];
}

FText AHM_Button::K2_GetLetter() const
{
	return TextRender->Text;
}

void AHM_Button::SetPressResult(bool bResult)
{
	bResultOnPress = bResult;
}

bool AHM_Button::CanBeInteractedWith(AActor *OtherActor) const
{
	const bool Result = 
		State != EHM_ButtonState::DisplayingError &&
		State != EHM_ButtonState::PushedSuccess &&
		State != EHM_ButtonState::PushedFailed;
	
	AGameModeBase *Gm = GetWorld()->GetAuthGameMode();
	if (!IsValid(Gm))
		return Result;

	const auto HmGm = CastChecked<AHM_GameModeBase>(Gm);
	return HmGm->GetGameState() == EHM_GameState::InProgress && Result;
}

void AHM_Button::Interact_Implementation(AActor *OtherActor)
{
	const bool bAllowInteraction = CanBeInteractedWith(OtherActor);
	Super::Interact_Implementation(OtherActor);

	if (bAllowInteraction)
	{
		SetState(bResultOnPress ?
			EHM_ButtonState::PushedSuccess : EHM_ButtonState::PushedFailed);
	}
	else
	{
		HandleInteractError();
	}
}

void AHM_Button::Reset()
{
	Super::Reset();

	SetState(EHM_ButtonState::Released);
	State = EHM_ButtonState::Released;
	
	bResultOnPress = true;
	
	if (DisplayErrorTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DisplayErrorTimerHandle);
	}
}

void AHM_Button::BeginPlay()
{
	Super::BeginPlay();
	SetState(State);
}

void AHM_Button::HandleInteractError()
{
	if (DisplayErrorTimerHandle.IsValid())
		return;
	
	GetWorldTimerManager().SetTimer(DisplayErrorTimerHandle, this,
		&ThisClass::StopDisplayingError, DisplayErrorSeconds);

	PostDisplayErrorState = State;
	SetState(EHM_ButtonState::DisplayingError);
}

void AHM_Button::SetState(EHM_ButtonState ButtonState)
{
	const auto Found = StateParameters.Find(ButtonState);
	if (!Found)
	{
		HMS_WARN("ButtonState [%d] couldn't be found", ButtonState);
		return;
	}

	if (!IsValid(Found->Material))
	{
		HMS_WARN("ButtonState's [%s] material is NULL", ButtonState);
		return;
	}

	StaticMesh->SetMaterial(0, Found->Material);
	TextRender->SetTextRenderColor(Found->TextColor);
	
	State = ButtonState;
}

void AHM_Button::StopDisplayingError()
{
	SetState(PostDisplayErrorState);
	DisplayErrorTimerHandle.Invalidate();
}
