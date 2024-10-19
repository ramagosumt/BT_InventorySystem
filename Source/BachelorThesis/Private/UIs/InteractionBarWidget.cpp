#include "UIs/InteractionBarWidget.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionBarWidget::UpdateWidget(const FInteractionData& InteractionData) const
{
	switch (InteractionData.ItemInteractionMode)
	{
	case (EInteractionMode::InventoryItem):
		if (InteractionData.Quantity < 2)
		{
			if (InteractionData.Duration > 0.f)
			{
				InteractionText->SetText(FText::Format(FText::FromString("Hold E for {0}s to {1} {2}"), static_cast<int32>(InteractionData.Duration), InteractionData.Action, InteractionData.Name));
			}
			else
			{
				InteractionText->SetText(FText::Format(FText::FromString("Press E to {0} {1}"), InteractionData.Action, InteractionData.Name));
			}
		}
		else
		{
			if (InteractionData.Duration > 0.f)
			{
				InteractionText->SetText(FText::Format(FText::FromString("Hold E for {0}s to {1} {2} {3}"), static_cast<int32>(InteractionData.Duration), InteractionData.Quantity, InteractionData.Action, InteractionData.Name));
			}
			else
			{
				InteractionText->SetText(FText::Format(FText::FromString("Press E to {0} {1} {2}"), InteractionData.Action, InteractionData.Quantity, InteractionData.Name));
			}
		}
		
		break;
		
	case (EInteractionMode::NPC):
		if (InteractionData.Duration > 0.f)
		{
			InteractionText->SetText(FText::Format(FText::FromString("Hold E for {0}s to {1} {2}"), static_cast<int32>(InteractionData.Duration), InteractionData.Action, InteractionData.Name));
		}
		else
		{
			InteractionText->SetText(FText::Format(FText::FromString("Press E to {0} {1}"), InteractionData.Action, InteractionData.Name));
		}
		
		break;
	case (EInteractionMode::Device):
		break;
	case (EInteractionMode::Toggle):
		break;
	case (EInteractionMode::Container):
		break;
	default:
		;
	}
}