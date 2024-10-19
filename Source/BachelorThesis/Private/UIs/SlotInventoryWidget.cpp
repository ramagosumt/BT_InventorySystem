#include "UIs/SlotInventoryWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "UIs/SlotInventorySlotWidget.h"
#include "Components/SlotInventoryComponent.h"
#include "Components/WrapBox.h"

void USlotInventoryWidget::InitializeWidget(USlotInventoryComponent* NewSlotInventory, const int32 NewTileSize)
{
	SlotInventory = NewSlotInventory;
	SlotSize = 2 * NewTileSize;

	const FVector2D WidgetSize = FVector2D(FVector2D(SlotSize * SlotInventory->GetSize(), SlotSize));

	if (SlotInventory)
	{
		SlotInventory->OnInventoryUpdated.AddUObject(this, &USlotInventoryWidget::RefreshWidget);
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot)) CanvasPanelSlot->SetSize(WidgetSize);

		if (DesiredSlotWidget)
		{
			for (int i = 0; i < SlotInventory->GetSize(); i++)
			{
				USlotInventorySlotWidget* SlotWidget = CreateWidget<USlotInventorySlotWidget>(this, DesiredSlotWidget);
				SlotWidget->SetSlotSize(SlotSize);
			
				if (InventoryPanel) InventoryPanel->AddChildToWrapBox(SlotWidget);
			}
		}
	}

	RefreshWidget();
}

void USlotInventoryWidget::RefreshWidget()
{
	if (SlotInventory && DesiredSlotWidget)
	{
		InventoryPanel->ClearChildren();

		for (const auto Item : SlotInventory->GetInventoryContent())
		{
			USlotInventorySlotWidget* SlotWidget = CreateWidget<USlotInventorySlotWidget>(this, DesiredSlotWidget);
			SlotWidget->SetSlotSize(SlotSize);
			SlotWidget->SetItemReference(Item);
			SlotWidget->OnQuestItemHovered.AddDynamic(this, &USlotInventoryWidget::HandleItemHovered);
			InventoryPanel->AddChildToWrapBox(SlotWidget);
		}
	}
}

void USlotInventoryWidget::HandleItemHovered(UBaseQuestItem* ItemBeingHovered)
{
	OnQuestSlotHovered.Broadcast(ItemBeingHovered);
}
