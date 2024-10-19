#include "UIs/PlayerMenuWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "UIs/GridInventoryDragDropOperation.h"
#include "UIs/GridInventoryWidget.h"
#include "UIs/SlotInventoryWidget.h"
#include "UIs/ListInventoryWidget.h"

void UPlayerMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GridInventory)
	{
		if (GridInventoryWidget)
		{
			if (UCanvasPanelSlot* InventoryPanel = Cast<UCanvasPanelSlot>(GridInventoryWidget->Slot))
			{
				const FVector2D DesiredSize = FVector2D(GridInventory->GetSizeX() * TileSize, GridInventory->GetSizeY() * TileSize);
				InventoryPanel->SetSize(DesiredSize);
			}

			GridInventoryWidget->InitializeWidget(GridInventory, TileSize);
			GridInventoryWidget->OnSlotHovered.AddUniqueDynamic(this, &UPlayerMenuWidget::UpdateItemInformation);
		}
	}

	if (SlotInventory)
	{
		if (SlotInventoryWidget)
		{
			SlotInventoryWidget->InitializeWidget(SlotInventory, TileSize);
			SlotInventoryWidget->OnQuestSlotHovered.AddUniqueDynamic(this, &UPlayerMenuWidget::UpdateQuestItemInformation);
		}
	}

	if (ListInventory)
	{
		if (ListInventoryWidget)
		{
			ListInventoryWidget->InitializeWidget(ListInventory);
		}
	}

	if (ItemName) ItemName->SetText(FText::GetEmpty());
	if (ItemDescription) ItemDescription->SetText(FText::GetEmpty());

	if (KeyItemsButton) KeyItemsButton->OnPressed.AddUniqueDynamic(this, &UPlayerMenuWidget::ToggleKeyItemsMenu);
	if (InventoryButton) InventoryButton->OnPressed.AddUniqueDynamic(this, &UPlayerMenuWidget::ToggleInventoryMenu);
	if (JournalsButton) JournalsButton->OnPressed.AddUniqueDynamic(this, &UPlayerMenuWidget::ToggleJournalMenu);
}

void UPlayerMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Collapsed)
	{
		if (GridInventoryWidget)
		{
			if (GridInventoryWidget->GetCurrentChoiceMenu()) GridInventoryWidget->ClearChoiceMenu();
		}
	}

	Super::SetVisibility(InVisibility);
}

FReply UPlayerMenuWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return Reply.Handled();
}

bool UPlayerMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UGridInventoryDragDropOperation* ItemDragDrop = Cast<UGridInventoryDragDropOperation>(InOperation);
	if (GridInventory && ItemDragDrop->GetSourceItem())
	{
		GridInventory->RemoveSingleInstanceOfItem(ItemDragDrop->GetSourceItem());
		return true;
	}

	return false;
}

void UPlayerMenuWidget::UpdateItemInformation(UBaseItem* Item)
{
	if (Item)
	{
		ItemName->SetText(Item->GetItemTextData().Name);
		ItemDescription->SetText(Item->GetItemTextData().Description);
	}
	else
	{
		ItemName->SetText(FText::GetEmpty());
		ItemDescription->SetText(FText::GetEmpty());
	}
}

void UPlayerMenuWidget::UpdateQuestItemInformation(UBaseQuestItem* Item)
{
	if (Item)
	{
		ItemName->SetText(Item->GetItemTextData().Name);
		ItemDescription->SetText(Item->GetItemTextData().Description);
	}
	else
	{
		ItemName->SetText(FText::GetEmpty());
		ItemDescription->SetText(FText::GetEmpty());
	}
}

void UPlayerMenuWidget::StopDragDrop() const
{
	if (GridInventoryWidget)
	{
		GridInventoryWidget->StopDragDrop();
	}
}

void UPlayerMenuWidget::ToggleKeyItemsMenu()
{
	if (GridInventoryWidget) GridInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (SlotInventoryWidget) SlotInventoryWidget->SetVisibility(ESlateVisibility::Visible);
	if (ListInventoryWidget) ListInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerMenuWidget::ToggleInventoryMenu()
{
	if (GridInventoryWidget) GridInventoryWidget->SetVisibility(ESlateVisibility::Visible);
	if (SlotInventoryWidget) SlotInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (ListInventoryWidget) ListInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerMenuWidget::ToggleJournalMenu()
{
	if (GridInventoryWidget) GridInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (SlotInventoryWidget) SlotInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (ListInventoryWidget) ListInventoryWidget->SetVisibility(ESlateVisibility::Visible);
}
