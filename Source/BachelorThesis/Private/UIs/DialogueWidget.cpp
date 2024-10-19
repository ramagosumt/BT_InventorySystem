#include "UIs/DialogueWidget.h"
#include "UIs/DialogueChoiceWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UDialogueWidget::UpdateWidget(const int32 DialogueIndex, const FText& CharacterName, const FText& Dialogue, const TArray<FText>& Choices) const
{
	if (OptionPanel) OptionPanel->ClearChildren();

	if (SpeakerName) SpeakerName->SetText(CharacterName);
	if (DialogueText) DialogueText->SetText(Dialogue);

	int32 ChoiceIndex = 0;
	for (const FText Choice : Choices)
	{
		if (DesiredChoiceWidget)
		{
			if (UDialogueChoiceWidget* ChoiceWidget = CreateWidget<UDialogueChoiceWidget>(GetOwningPlayer(), DesiredChoiceWidget))
			{
				ChoiceWidget->SetOwningDialogueComponent(OwningDialogueComponent);
				ChoiceWidget->SetChoiceText(Choice);
				ChoiceWidget->SetChoiceIndex(ChoiceIndex);
				ChoiceWidget->SetDialogueIndex(DialogueIndex);
				
				OptionPanel->AddChild(ChoiceWidget);
			}

			ChoiceIndex++;
		}
	}
}
