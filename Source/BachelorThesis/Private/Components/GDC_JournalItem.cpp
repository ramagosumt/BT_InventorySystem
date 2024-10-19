#include "Components/GDC_JournalItem.h"

void UGDC_JournalItem::ExecuteDialogue()
{
	switch (DialogueTreeBranch)
	{
	case 0:
		{
			int32 ChoiceIndex = 0;
	
			TArray<FText> Choices;
			Choices.Add(FText::FromString("Continue"));

			EDialogueState StateOne = AddDialogue(FText::FromName("Rama"), FText::FromString("This is a journal item."), Choices, ChoiceIndex);

			switch (StateOne)
			{
			case EDialogueState::PassThrough:
				{
					switch (ChoiceIndex)
					{
					case 0:
						{
							CloseConversation();
							OnDialogueEnd.Broadcast();
							break;
						}
	
					default:
						break;
					}
				}
			case EDialogueState::Updated:
				{
					break;
				}
			}
	
			break;
		}
	
	default:
		break;
	}
}
