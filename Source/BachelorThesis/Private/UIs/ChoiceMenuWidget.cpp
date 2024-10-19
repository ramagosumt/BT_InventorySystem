#include "UIs/ChoiceMenuWidget.h"
#include "Actors/Grid3DItemSlot.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridInventoryComponent.h"
#include "UIs/GridInventorySlotWidget.h"

void UChoiceMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChoiceMenu)
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ChoiceMenu->Slot))
		{
			if (OwningSlot)
			{
				CanvasPanelSlot->SetPosition(OwningSlot->GetCenterRightPositionInViewport());
			}
			else if (Owning3DSlot)
			{
				FWidgetTransform WidgetTransform;
				WidgetTransform.Translation = Owning3DSlot->GetCenterRightPositionInViewport();
				SetRenderTransform(WidgetTransform);
			}
		}
	}

	if (UseButton) UseButton->OnPressed.AddUniqueDynamic(this, &UChoiceMenuWidget::UseItem);
	if (DropButton) DropButton->OnPressed.AddUniqueDynamic(this, &UChoiceMenuWidget::DropItem);
	if (DiscardButton) DiscardButton->OnPressed.AddUniqueDynamic(this, &UChoiceMenuWidget::DiscardItemSlot);
}

FReply UChoiceMenuWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (OwningSlot) OwningSlot->NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	}

	return Reply.Handled();
}

void UChoiceMenuWidget::UseItem()
{
	
}

void UChoiceMenuWidget::DropItem()
{
	
}

void UChoiceMenuWidget::DiscardItemSlot()
{
	if (OwningSlot) OwningSlot->GetInventoryReference()->RemoveSingleInstanceOfItem(OwningSlot->GetItemReference());
		else if (Owning3DSlot) Owning3DSlot->GetInventoryReference()->RemoveSingleInstanceOfItem(Owning3DSlot->GetItemReference());
}
