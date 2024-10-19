#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListInventorySlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJournalItemHovered, UBaseJournalItem*, ItemReference);

class UButton;
class UTextBlock;
class UBaseJournalItem;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UListInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UButton* SlotButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY()
	UBaseJournalItem* ItemReference;

	void InitializeWidget();

public:
	FOnJournalItemHovered OnJournalItemHovered;
	
	FORCEINLINE void SetItemReference(UBaseJournalItem* NewItemReference) { ItemReference = NewItemReference; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
};
