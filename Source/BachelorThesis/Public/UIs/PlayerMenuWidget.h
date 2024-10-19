#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenuWidget.generated.h"

class UGridInventoryComponent;
class USlotInventoryComponent;
class UListInventoryComponent;
class UListInventoryWidget;
class UCanvasPanel;
class UGridInventoryWidget;
class USlotInventoryWidget;
class UTextBlock;
class UButton;
class UBaseItem;
class UBaseQuestItem;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UPlayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UGridInventoryComponent* GridInventory;

	UPROPERTY()
	USlotInventoryComponent* SlotInventory;

	UPROPERTY()
	UListInventoryComponent* ListInventory;

	int32 TileSize;

	UPROPERTY(meta=(BindWidget))
	UGridInventoryWidget* GridInventoryWidget;

	UPROPERTY(meta=(BindWidget))
	USlotInventoryWidget* SlotInventoryWidget;

	UPROPERTY(meta=(BindWidget))
	UListInventoryWidget* ListInventoryWidget;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta=(BindWidget))
	UButton* InventoryButton;

	UPROPERTY(meta=(BindWidget))
	UButton* KeyItemsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* JournalsButton;

	UFUNCTION()
	void UpdateItemInformation(UBaseItem* Item);

	UFUNCTION()
	void UpdateQuestItemInformation(UBaseQuestItem* Item);

	UFUNCTION()
	void ToggleKeyItemsMenu();

	UFUNCTION()
	void ToggleInventoryMenu();

	UFUNCTION()
	void ToggleJournalMenu();

public:
	void StopDragDrop() const;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
	FORCEINLINE void SetGridInventory(UGridInventoryComponent* NewGridInventory) { GridInventory = NewGridInventory; }
	FORCEINLINE void SetSlotInventory(USlotInventoryComponent* NewSlotInventory) { SlotInventory = NewSlotInventory; }
	FORCEINLINE void SetListInventory(UListInventoryComponent* NewListInventory) { ListInventory = NewListInventory; }
	FORCEINLINE void SetTileSize(const int32 NewTileSize) { TileSize = NewTileSize; }

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	
};
