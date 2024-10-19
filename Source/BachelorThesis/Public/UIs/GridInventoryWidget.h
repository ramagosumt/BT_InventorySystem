#pragma once

#include "CoreMinimal.h"
#include "ChoiceMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridInventoryComponent.h"
#include "GridInventoryWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, UBaseItem*, ItemReference);

class UCanvasPanel;
class UBorder;
class UGridInventorySlotWidget;
class USlateBrushAsset;
class UChoiceMenuWidget;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UGridInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UGridInventoryComponent* GridInventory;
	
	float TileSize;
	TArray<FLine> Lines;

	UPROPERTY()
	UBaseItem* DraggedItem;
	
	FIntPoint DraggedItemTopLeftTile;
	
	bool bIsEligibleToDraw;

	UPROPERTY(meta=(BindWidget))
	UBorder* GridBorder;

	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* GridPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGridInventorySlotWidget> DesiredInventorySlotUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USlateBrushAsset> DesiredBrushAsset;

	UPROPERTY()
	UChoiceMenuWidget* CurrentChoiceMenu;

	void CreateLineSegments();
	void RefreshWidget();
	void FindMousePositionInTile(const FVector2D MousePosition, bool& IsRight, bool& IsBottom) const;

	UFUNCTION()
	void HandleItemHovered(UBaseItem* ItemBeingHovered);

public:
	FOnSlotHovered OnSlotHovered;
	
	void InitializeWidget(UGridInventoryComponent* NewGridInventory, const int32 NewTileSize);
	void StopDragDrop();

	FORCEINLINE UChoiceMenuWidget* GetCurrentChoiceMenu() const { return CurrentChoiceMenu; }
	FORCEINLINE void SetCurrentChoiceMenu(UChoiceMenuWidget* NewChoiceMenu) { CurrentChoiceMenu = NewChoiceMenu; }
	FORCEINLINE void ClearChoiceMenu() { CurrentChoiceMenu->RemoveFromParent(); CurrentChoiceMenu = nullptr; }

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
		const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                              UDragDropOperation* InOperation) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
};
