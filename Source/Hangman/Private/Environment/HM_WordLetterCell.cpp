#include "Environment/HM_WordLetterCell.h"

#include "Components/TextRenderComponent.h"

AHM_WordLetterCell::AHM_WordLetterCell()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	NoLetterTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("No Letter Text"));
	NoLetterTextRender->SetupAttachment(StaticMesh);
	
	LetterTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Letter Text"));
	LetterTextRender->SetupAttachment(StaticMesh);
}

void AHM_WordLetterCell::SetLetter(TCHAR Letter) const
{
	FString String;
	String.AppendChar(Letter);
	
	LetterTextRender->SetText(FText::FromString(String));
}

TCHAR AHM_WordLetterCell::GetLetter() const
{
	const FString String = LetterTextRender->Text.ToString();
	check(String.Len() == 1);
	return String[0];
}

void AHM_WordLetterCell::Reveal_Implementation()
{
}

void AHM_WordLetterCell::BeginPlay()
{
	Super::BeginPlay();

	NoLetterTextRender->SetText(NoLetterText);
}
