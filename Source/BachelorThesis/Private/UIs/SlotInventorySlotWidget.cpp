#include "UIs/SlotInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/BaseQuestItem.h"

void USlotInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();
}

void USlotInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ItemBorder) ItemBorder->SetBrushColor(UKismetMathLibrary::MakeColor(0.f, 0.f, 0.f, .5f));
	if (ItemReference) OnQuestItemHovered.Broadcast(ItemReference);
}

void USlotInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (ItemBorder) ItemBorder->SetBrushColor(UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .2f));
	OnQuestItemHovered.Broadcast(nullptr);
}

void USlotInventorySlotWidget::InitializeWidget()
{
	if (ItemSizeBox)
	{
		ItemSizeBox->SetWidthOverride(SlotSize);
		ItemSizeBox->SetHeightOverride(SlotSize);
	}

	if (ItemBorder) ItemBorder->SetBrushColor(UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .2f));
	
	if (ItemReference)
	{
		if (ItemImage)
		{
			ItemImage->SetBrushFromTexture(ItemReference->GetItemAssetData().Icon);
		}
	}
	else
	{
		if (ItemImage) ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
