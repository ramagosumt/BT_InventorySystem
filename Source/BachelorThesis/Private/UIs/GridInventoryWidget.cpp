#include "UIs/GridInventoryWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridInventoryComponent.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "UIs/GridInventoryDragDropOperation.h"
#include "UIs/GridInventorySlotWidget.h"
#include "UIs/GridSlotDragVisualWidget.h"

int32 UGridInventoryWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                        const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	const FVector2D BorderTopLeft = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2f(0.f));
	
	for (const FLine Line : Lines)
	{
		UWidgetBlueprintLibrary::DrawLine(PaintContext, Line.Start + BorderTopLeft, Line.End + BorderTopLeft, UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .5f));
	}
	
	if (UWidgetBlueprintLibrary::IsDragDropping() && bIsEligibleToDraw && DraggedItem && DesiredBrushAsset)
	{
		const FVector2D DrawPosition = FVector2D(DraggedItemTopLeftTile.X * TileSize, DraggedItemTopLeftTile.Y * TileSize);
		const FVector2D DrawSize = FVector2D(DraggedItem->GetItemDimensions().X * TileSize, DraggedItem->GetItemDimensions().Y * TileSize);
		const int32 DraggedItemTopLeftIndex = GridInventory->TileToIndex(FTile(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y));
		
		if (GridInventory->IsSpaceAvailable(DraggedItem, DraggedItemTopLeftIndex))
		{
			UWidgetBlueprintLibrary::DrawBox(PaintContext, DrawPosition, DrawSize, DesiredBrushAsset.GetDefaultObject(), UKismetMathLibrary::MakeColor(0.f, 1.f, 0.f, .25f));
		}
		else
		{
			UWidgetBlueprintLibrary::DrawBox(PaintContext, DrawPosition, DrawSize, DesiredBrushAsset.GetDefaultObject(), UKismetMathLibrary::MakeColor(1.f, 0.f, 0.f, .25f));
		}
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							  bParentEnabled);
}

bool UGridInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (const UGridInventoryDragDropOperation* ItemDragDrop = Cast<UGridInventoryDragDropOperation>(InOperation))
	{
		if (GridInventory && ItemDragDrop->GetSourceItem())
		{
			DraggedItem = ItemDragDrop->GetSourceItem();
			const int32 DraggedItemTopLeftIndex = GridInventory->TileToIndex(FTile(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y));
			
			if (GridInventory->IsSpaceAvailable(DraggedItem, DraggedItemTopLeftIndex))
			{
				GridInventory->AddNewItem(DraggedItem, DraggedItemTopLeftIndex, DraggedItem->GetItemQuantity());
				DraggedItem = nullptr;
				return true;
			}
			
			FItemAddResult AddResult = GridInventory->HandleAddItem(DraggedItem);
			if (AddResult == FItemAddResult::AddedAll(DraggedItem->GetItemQuantity())) UE_LOG(LogTemp, Warning, TEXT("DUH"))
			DraggedItem = nullptr;
			return true;
		}
	}

	return false;
}

void UGridInventoryWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	bIsEligibleToDraw = true;
}

void UGridInventoryWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	bIsEligibleToDraw = false;
}

bool UGridInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                            UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	const FVector2D MousePosition = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(InDragDropEvent));
	
	bool IsRight = false;
	bool IsBottom = false;
	FindMousePositionInTile(MousePosition, IsRight, IsBottom);

	int32 RightPosition = 0;
	int32 BottomPosition = 0;

	DraggedItem = Cast<UGridInventoryDragDropOperation>(InOperation)->GetSourceItem();
	if (DraggedItem)
	{
		if (IsRight) RightPosition = FMath::Clamp(RightPosition, 0.f, DraggedItem->GetItemDimensions().X - 1);
			else RightPosition = FMath::Clamp(RightPosition, 0.f, DraggedItem->GetItemDimensions().X);
		if (IsBottom) BottomPosition = FMath::Clamp(BottomPosition, 0.f, DraggedItem->GetItemDimensions().X - 1);
			else BottomPosition = FMath::Clamp(BottomPosition, 0.f, DraggedItem->GetItemDimensions().X);
	}

	const FIntPoint FindDesiredPosition = FIntPoint(RightPosition, BottomPosition) / 2;
	DraggedItemTopLeftTile = FIntPoint(FMath::TruncToInt32(MousePosition.X / TileSize), FMath::TruncToInt32(MousePosition.Y / TileSize)) - FindDesiredPosition;

	return true;
}

FReply UGridInventoryWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::R)
	{
		if (DraggedItem)
		{
			DraggedItem->SetItemRotated();
			Cast<UGridSlotDragVisualWidget>(UWidgetBlueprintLibrary::GetDragDroppingContent()->DefaultDragVisual)->RefreshWidget();
		}
		
		return Reply.Handled();
	}

	return Reply.Unhandled();
}

void UGridInventoryWidget::InitializeWidget(UGridInventoryComponent* NewGridInventory, const int32 NewTileSize)
{
	GridInventory = NewGridInventory;
	TileSize = NewTileSize;

	if (GridInventory) GridInventory->OnInventoryUpdated.AddUObject(this, &UGridInventoryWidget::RefreshWidget);

	const FVector2D WidgetSize = FVector2D(GridInventory->GetSizeX() * TileSize, GridInventory->GetSizeY() * TileSize);
	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot)) CanvasPanelSlot->SetSize(WidgetSize);

	CreateLineSegments();

	RefreshWidget();
}

void UGridInventoryWidget::CreateLineSegments()
{
	if (GridInventory)
	{
		for (int FirstIndex = 0; FirstIndex < GridInventory->GetSizeX() + 1; FirstIndex++)
		{
			const float LineX = FirstIndex * TileSize;

			FLine Line = FLine(FVector2D(LineX, 0.f), FVector2D(LineX, GridInventory->GetSizeY() * TileSize));
			Lines.Add(Line);
		}

		for (int FirstIndex = 0; FirstIndex < GridInventory->GetSizeY() + 1; FirstIndex++)
		{
			const float LineY = FirstIndex * TileSize;

			FLine Line = FLine(FVector2D(0.f, LineY), FVector2D(GridInventory->GetSizeX() * TileSize, LineY));
			Lines.Add(Line);
		}
	}
}

void UGridInventoryWidget::RefreshWidget()
{
	if (GridInventory && DesiredInventorySlotUI)
	{
		GridPanel->ClearChildren();
		
		TArray<UBaseItem*> GridInventoryKeys;
		GridInventory->GetAllItems().GetKeys(GridInventoryKeys);
		
		for (const auto Item : GridInventoryKeys)
		{
			const FTile* TopLeftTile = GridInventory->GetAllItems().Find(Item);
			const int32 TileX = TopLeftTile->X;
			const int32 TileY = TopLeftTile->Y;
			
			UGridInventorySlotWidget* SlotWidget = CreateWidget<UGridInventorySlotWidget>(this, DesiredInventorySlotUI);
			SlotWidget->SetTileSize(TileSize);
			SlotWidget->SetOwningWidget(this);
			SlotWidget->SetInventoryReference(GridInventory);
			SlotWidget->SetItemReference(Item);
			SlotWidget->OnItemHovered.AddDynamic(this, &UGridInventoryWidget::HandleItemHovered);

			UCanvasPanelSlot* ChildSlotWidget = Cast<UCanvasPanelSlot>(GridPanel->AddChild(SlotWidget));
			ChildSlotWidget->SetAutoSize(true);

			const float ChildPositionX = TileX * TileSize;
			const float ChildPositionY = TileY * TileSize;
			
			ChildSlotWidget->SetPosition(FVector2D(ChildPositionX, ChildPositionY));
		}
	}
}

void UGridInventoryWidget::FindMousePositionInTile(const FVector2D MousePosition, bool& IsRight, bool& IsBottom) const
{
	double RightRemainder = 0;
	UKismetMathLibrary::FMod(MousePosition.X, TileSize, RightRemainder);
	IsRight = (RightRemainder > TileSize / 2);

	double BottomRemainder = 0;
	UKismetMathLibrary::FMod(MousePosition.X, TileSize, BottomRemainder);
	IsBottom = (BottomRemainder > TileSize / 2);
}

void UGridInventoryWidget::HandleItemHovered(UBaseItem* ItemBeingHovered)
{
	OnSlotHovered.Broadcast(ItemBeingHovered);
}

void UGridInventoryWidget::StopDragDrop()
{
	if (GridInventory && DraggedItem)
	{
		GridInventory->HandleAddItem(DraggedItem);
		DraggedItem = nullptr;
		UWidgetBlueprintLibrary::CancelDragDrop();
	}
}
