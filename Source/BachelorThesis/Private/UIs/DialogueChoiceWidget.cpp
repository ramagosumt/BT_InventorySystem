#include "UIs/DialogueChoiceWidget.h"
#include "Components/Button.h"
#include "Components/GraphDialogueComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UDialogueChoiceWidget::NativeConstruct()
{
	if (BackgroundImage) BackgroundImage->SetBrushTintColor(UKismetMathLibrary::MakeColor(0.f, 0.f, 0.f, 0.5f));
	if (ChoiceDialogue) ChoiceDialogue->SetText(ChoiceText);

	if (ChoiceButton)
	{
		ChoiceButton->OnHovered.AddUniqueDynamic(this, &UDialogueChoiceWidget::OnChoiceHovered);
		ChoiceButton->OnUnhovered.AddUniqueDynamic(this, &UDialogueChoiceWidget::OnChoiceUnhovered);
		ChoiceButton->OnReleased.AddUniqueDynamic(this, &UDialogueChoiceWidget::UpdateDialogueComponent);
	}
}

void UDialogueChoiceWidget::OnChoiceHovered()
{
	if (BackgroundImage) BackgroundImage->SetBrushTintColor(UKismetMathLibrary::MakeColor(0.5f, 0.5f, 0.5f, 0.2f));
}

void UDialogueChoiceWidget::OnChoiceUnhovered()
{
	if (BackgroundImage) BackgroundImage->SetBrushTintColor(UKismetMathLibrary::MakeColor(0.f, 0.f, 0.f, 0.5f));
}

void UDialogueChoiceWidget::UpdateDialogueComponent()
{
	if (OwningDialogueComponent) OwningDialogueComponent->UpdatePath(DialogueIndex, ChoiceIndex);
}
