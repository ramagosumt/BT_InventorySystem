#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridSlotDragVisualWidget.generated.h"

class USizeBox;
class UBorder;
class UImage;
class UTextBlock;
class UBaseItem;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UGridSlotDragVisualWidget : public UUserWidget
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

	UPROPERTY()
	UBaseItem* ItemReference;

	float TileSize;
	FVector2D Size;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* DesiredItemImageMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ItemImageMaterial;

public:
	void RefreshWidget();

	FORCEINLINE void SetTileSize(const float NewTileSize) { TileSize = NewTileSize; }
	FORCEINLINE void SetItemReference(UBaseItem* NewItemReference) { ItemReference = NewItemReference; }
	
	FORCEINLINE USizeBox* GetBackgroundSizeBox() const { return BackgroundSizeBox; }
	FORCEINLINE UBorder* GetBackgroundBorder() const { return BackgroundBorder; }
	FORCEINLINE UImage* GetItemImage() const { return ItemImage; }
	FORCEINLINE UTextBlock* GetItemQuantity() const { return ItemQuantity; }
	
};
