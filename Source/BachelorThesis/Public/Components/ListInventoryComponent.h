#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ListInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated)

class UBaseJournalItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UListInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInventoryUpdated OnInventoryUpdated;
	
	UListInventoryComponent();

	void AddItem(UBaseJournalItem* ItemToAdd);
	void RemoveItem(UBaseJournalItem* ItemToRemove);

	FORCEINLINE TArray<UBaseJournalItem*> GetInventoryContent() { return InventoryContent; }

protected:
	UPROPERTY(VisibleAnywhere, Category= "Defaults")
	TArray<UBaseJournalItem*> InventoryContent;

	void SortAlphabet_Ascend();
	void SortAlphabet_Descend();
		
};
