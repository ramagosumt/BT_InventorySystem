#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "GridInventoryDragDropOperation.generated.h"

class UBaseItem;
class UGridInventoryComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UGridInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

	UPROPERTY()
	UBaseItem* SourceItem;
	
	UPROPERTY()
	UGridInventoryComponent* GridInventory;

public:
	FORCEINLINE UBaseItem* GetSourceItem() const { return SourceItem; }
	FORCEINLINE UGridInventoryComponent* GetGridInventory() const { return GridInventory; }
	
	FORCEINLINE void SetSourceItem(UBaseItem* NewSourceItem) { SourceItem = NewSourceItem; }
	FORCEINLINE void SetGridInventory(UGridInventoryComponent* NewGridInventory) { GridInventory = NewGridInventory; }
	
};
