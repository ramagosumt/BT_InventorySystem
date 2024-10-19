#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlotInventoryComponent.h"
#include "SlotInventoryWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestSlotHovered, UBaseQuestItem*, ItemReference);

class UWrapBox;
class USlotInventoryComponent;
class USlotInventorySlotWidget;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API USlotInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY()
	USlotInventoryComponent* SlotInventory;

	float SlotSize;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USlotInventorySlotWidget> DesiredSlotWidget;

	void RefreshWidget();

	UFUNCTION()
	void HandleItemHovered(UBaseQuestItem* ItemBeingHovered);

public:
	FOnQuestSlotHovered OnQuestSlotHovered;
	
	void InitializeWidget(USlotInventoryComponent* NewSlotInventory, const int32 NewTileSize);
	
};
