#pragma once

#include "CoreMinimal.h"
#include "Data/BaseItemData.h"
#include "BaseQuestItem.generated.h"

class USlotInventoryComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UBaseQuestItem : public UObject
{
	GENERATED_BODY()

public:
	UBaseQuestItem();

	FORCEINLINE USlotInventoryComponent* GetOwningInventory() const { return OwningInventory; }
	FORCEINLINE FName GetItemID() const { return ID; }
	FORCEINLINE EItemType GetItemType() const { return Type; }
	FORCEINLINE FItemTextData GetItemTextData() { return TextData; }
	FORCEINLINE FItemAssetData GetItemAssetData() const { return AssetData; }
	
	FORCEINLINE void SetOwningInventory(USlotInventoryComponent* NewSlotInventory) { OwningInventory = NewSlotInventory; }
	FORCEINLINE void SetItemID(const FName NewID) { ID = NewID; }
	FORCEINLINE void SetItemType(const EItemType NewType) { Type = NewType; }
	FORCEINLINE void SetItemTextData(const FItemTextData& NewTextData) { TextData = NewTextData; }
	FORCEINLINE void SetItemAssetData(const FItemAssetData& NewAssetData) { AssetData = NewAssetData; }

protected:
	UPROPERTY()
	USlotInventoryComponent* OwningInventory;

	FName ID;
	EItemType Type;
	FItemTextData TextData;
	FItemAssetData AssetData;

	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
