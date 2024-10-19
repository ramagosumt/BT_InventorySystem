#include "UIs/ListInventoryWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ListInventoryComponent.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Objects/BaseJournalItem.h"
#include "UIs/ListInventorySlotWidget.h"

void UListInventoryWidget::InitializeWidget(UListInventoryComponent* NewListInventory)
{
	ListInventory = NewListInventory;

	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot)) CanvasPanelSlot->SetSize(FVector2D(2000.f, 750.f));
	if (ListInventory) ListInventory->OnInventoryUpdated.AddUObject(this, &UListInventoryWidget::RefreshWidget);

	RefreshWidget();
}

void UListInventoryWidget::RefreshWidget()
{
	if (ListInventory && DesiredSlotWidget)
	{
		InventoryPanel->ClearChildren();

		for (const auto Item : ListInventory->GetInventoryContent())
		{
			UListInventorySlotWidget* SlotWidget = CreateWidget<UListInventorySlotWidget>(this, DesiredSlotWidget);
			SlotWidget->SetItemReference(Item);
			SlotWidget->OnJournalItemHovered.AddDynamic(this, &UListInventoryWidget::HandleItemHovered);
			InventoryPanel->AddChild(SlotWidget);
		}
	}
}

void UListInventoryWidget::HandleItemHovered(UBaseJournalItem* ItemBeingHovered)
{
	if (ItemBeingHovered)
	{
		if (JournalText) JournalText->SetText(ItemBeingHovered->GetItemTextData().Description);
	}
	else
	{
		if (JournalText) JournalText->SetText(FText::GetEmpty());
	}
}