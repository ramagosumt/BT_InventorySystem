#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotInventorySlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestItemHovered, UBaseQuestItem*, ItemReference);

class USizeBox;
class UBorder;
class UImage;
class UBaseQuestItem;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API USlotInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	USizeBox* ItemSizeBox;

	UPROPERTY(meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;

	float SlotSize;

	UPROPERTY()
	UBaseQuestItem* ItemReference;

	void InitializeWidget();

public:
	FOnQuestItemHovered OnQuestItemHovered;
	
	FORCEINLINE void SetSlotSize(const float NewSlotSize) { SlotSize = NewSlotSize; }
	FORCEINLINE void SetItemReference(UBaseQuestItem* NewItemReference) { ItemReference = NewItemReference; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
};
