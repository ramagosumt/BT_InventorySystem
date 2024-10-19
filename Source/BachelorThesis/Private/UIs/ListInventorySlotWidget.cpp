#include "UIs/ListInventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/BaseJournalItem.h"

void UListInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();
}

void UListInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (SlotButton) SlotButton->SetBackgroundColor(UKismetMathLibrary::MakeColor(.0f, .0f, .0f, .5f));
	if (ItemReference) OnJournalItemHovered.Broadcast(ItemReference);
}

void UListInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (SlotButton) SlotButton->SetBackgroundColor(UKismetMathLibrary::MakeColor(.0f, .0f, .0f, .0f));
	OnJournalItemHovered.Broadcast(nullptr);
}

void UListInventorySlotWidget::InitializeWidget()
{
	if (SlotButton) SlotButton->SetBackgroundColor(UKismetMathLibrary::MakeColor(.0f, .0f, .0f, .0f));
	
	if (ItemReference) if (ItemName) ItemName->SetText(ItemReference->GetItemTextData().Name);
}
