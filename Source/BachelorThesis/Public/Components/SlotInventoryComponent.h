#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/BaseQuestItem.h"
#include "SlotInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated)

class UBaseQuestItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API USlotInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 InventorySize;

public:
	FOnInventoryUpdated OnInventoryUpdated;
	
	USlotInventoryComponent();

	void AddItem(UBaseQuestItem* ItemToAdd);
	void RemoveItem(UBaseQuestItem* ItemToRemove);

	FORCEINLINE int32 GetSize() const { return InventorySize; }
	FORCEINLINE TArray<UBaseQuestItem*> GetInventoryContent() { return InventoryContent; }
	
	FORCEINLINE void SetSize(const int32 NewInventorySize) { InventorySize = NewInventorySize; }

protected:
	UPROPERTY(VisibleAnywhere, Category= "Defaults")
	TArray<UBaseQuestItem*> InventoryContent;
	
	virtual void BeginPlay() override;
	
};
