#include "UIs/GridInventorySlotWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridInventoryComponent.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/BaseItem.h"
#include "UIs/GridSlotDragVisualWidget.h"
#include "UIs/GridInventoryDragDropOperation.h"
#include "UIs/ChoiceMenuWidget.h"
#include "UIs/GridInventoryWidget.h"

void UGridInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();
}

void UGridInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	BackgroundBorder->SetBrushColor(UKismetMathLibrary::MakeColor(0.f, 0.f, 0.f, .5f));
	if (ItemReference) OnItemHovered.Broadcast(ItemReference);
}

void UGridInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	BackgroundBorder->SetBrushColor(UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .2f));
	OnItemHovered.Broadcast(nullptr);
}

FReply UGridInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (ItemReference)
		{
			if (DesiredChoiceMenuClass)
			{
				if (ChoiceMenuWidget) ChoiceMenuWidget->RemoveFromParent();
                    
				ChoiceMenuWidget = CreateWidget<UChoiceMenuWidget>(GetWorld(), DesiredChoiceMenuClass);
				if (ChoiceMenuWidget)
				{
					ChoiceMenuWidget->SetOwningSlot(this);
					if (OwningWidget)
					{
						if (OwningWidget->GetCurrentChoiceMenu()) OwningWidget->ClearChoiceMenu();
						OwningWidget->SetCurrentChoiceMenu(ChoiceMenuWidget);
					}
					ChoiceMenuWidget->AddToViewport();
				}
			}
		}

		SetIsFocusable(true);
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	SetIsFocusable(false);
	return Reply.Unhandled();
}

FReply UGridInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	SetIsFocusable(false);
	
	return Reply.Handled();
}

void UGridInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                    UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (ItemReference && SlotDragVisualClass)
	{
		UGridSlotDragVisualWidget* DragItem = CreateWidget<UGridSlotDragVisualWidget>(this, SlotDragVisualClass);
		DragItem->SetTileSize(TileSize);
		DragItem->SetItemReference(ItemReference);
		DragItem->RefreshWidget();

		UGridInventoryDragDropOperation* DragItemOperation = NewObject<UGridInventoryDragDropOperation>();
		DragItemOperation->SetSourceItem(ItemReference);
		DragItemOperation->SetGridInventory(ItemReference->GetOwningInventory());

		DragItemOperation->DefaultDragVisual = Cast<UWidget>(DragItem);
		DragItemOperation->Pivot = EDragPivot::CenterLeft;

		OutOperation = DragItemOperation;

		ItemReference->GetOwningInventory()->RemoveSingleInstanceOfItem(ItemReference);
		this->RemoveFromParent();
	}
}

void UGridInventorySlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ChoiceMenuWidget) ChoiceMenuWidget->RemoveFromParent();
	if (OwningWidget)
	{
		if (OwningWidget->GetCurrentChoiceMenu()) OwningWidget->ClearChoiceMenu();
	}
}

void UGridInventorySlotWidget::InitializeWidget()
{
	if (ItemReference)
	{
		BackgroundBorder->SetBrushColor(UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .2f));
		
		const float WidgetSizeX = ItemReference->GetItemDimensions().X * TileSize;
		const float WidgetSizeY = ItemReference->GetItemDimensions().Y * TileSize;

		Size = FVector2D(WidgetSizeX, WidgetSizeY);

		BackgroundSizeBox->SetWidthOverride(Size.X);
		BackgroundSizeBox->SetHeightOverride(Size.Y);
		
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ItemImage->Slot)) CanvasPanelSlot->SetSize(Size);
		
		if (ItemImage)
		{
			if (!IsValid(ItemImageMaterial))
			{
				if (DesiredItemImageMaterial)
				{
					ItemImageMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DesiredItemImageMaterial);
					if (ItemImageMaterial)
					{
						ItemImageMaterial->SetTextureParameterValue(TEXT("ItemImage"), ItemReference->GetItemAssetData().Icon);
						if (ItemReference->GetItemRotated()) ItemImageMaterial->SetScalarParameterValue(TEXT("RotationAngle"), .75f);
							else ItemImageMaterial->SetScalarParameterValue(TEXT("RotationAngle"), 0.f);

						ItemImage->SetBrushFromMaterial(ItemImageMaterial);
					}
					else
					{
						if (ItemReference->GetItemRotated()) ItemImage->SetRenderTransformAngle(90.f);
							else ItemImage->SetRenderTransformAngle(0.f);
						
						ItemImage->SetBrushFromTexture(ItemReference->GetItemAssetData().Icon);
					}
				}
			}
		}

		if (ItemReference->GetItemNumericData().bIsStackable)
		{
			if (ItemQuantity) ItemQuantity->SetText(FText::AsNumber(ItemReference->GetItemQuantity()));
		}
		else
		{
			if (ItemQuantity) ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FVector2D UGridInventorySlotWidget::GetCenterRightPositionInViewport() const
{
	const FGeometry WidgetGeometry = GetCachedGeometry();

	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	
	USlateBlueprintLibrary::LocalToViewport(GetWorld(), WidgetGeometry, FVector2D(0.f), PixelPosition, ViewportPosition);

	return FVector2D(ViewportPosition.X + WidgetGeometry.GetLocalSize().X, ViewportPosition.Y + WidgetGeometry.GetLocalSize().Y * 0.5);
}
