#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListInventoryWidget.generated.h"

class UScrollBox;
class UTextBlock;
class UListInventoryComponent;
class UListInventorySlotWidget;
class UBaseJournalItem;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UListInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UScrollBox* InventoryPanel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* JournalText;

	UPROPERTY()
	UListInventoryComponent* ListInventory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UListInventorySlotWidget> DesiredSlotWidget;

	void RefreshWidget();

	UFUNCTION()
	void HandleItemHovered(UBaseJournalItem* ItemBeingHovered);

public:
	void InitializeWidget(UListInventoryComponent* NewListInventory);
	
};
