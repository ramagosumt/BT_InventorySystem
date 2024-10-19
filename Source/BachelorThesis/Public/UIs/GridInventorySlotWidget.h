#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridInventorySlotWidget.generated.h"

class UGridInventoryWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemHovered, UBaseItem*, ItemReference);

class USizeBox;
class UBorder;
class UImage;
class UTextBlock;
class UGridInventoryComponent;
class UBaseItem;
class UGridSlotDragVisualWidget;
class UChoiceMenuWidget;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UGridInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	USizeBox* BackgroundSizeBox;

	UPROPERTY(meta=(BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantity;

	float TileSize;

	FVector2D Size;

	UPROPERTY()
	UGridInventoryWidget* OwningWidget;

	UPROPERTY()
	UGridInventoryComponent* InventoryReference;

	UPROPERTY()
	UBaseItem* ItemReference;

	UPROPERTY()
	UChoiceMenuWidget* ChoiceMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category= "Inventory slot")
	TSubclassOf<UGridSlotDragVisualWidget> SlotDragVisualClass;

	UPROPERTY(EditDefaultsOnly, Category= "Inventory slot")
	TSubclassOf<UChoiceMenuWidget> DesiredChoiceMenuClass;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* DesiredItemImageMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ItemImageMaterial;

	void InitializeWidget();

public:
	FOnItemHovered OnItemHovered;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FVector2D GetCenterRightPositionInViewport() const;

	FORCEINLINE UGridInventoryComponent* GetInventoryReference() const { return InventoryReference; }
	FORCEINLINE UBaseItem* GetItemReference() const { return ItemReference; }
	
	FORCEINLINE void SetTileSize(const float NewTileSize) { TileSize = NewTileSize; }
	FORCEINLINE void SetOwningWidget(UGridInventoryWidget* NewOwningWidget) { OwningWidget = NewOwningWidget; }
	FORCEINLINE void SetInventoryReference(UGridInventoryComponent* NewInventoryReference) { InventoryReference = NewInventoryReference; }
	FORCEINLINE void SetItemReference(UBaseItem* NewItemReference) { ItemReference = NewItemReference; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;
	virtual void NativeDestruct() override;
	
};
