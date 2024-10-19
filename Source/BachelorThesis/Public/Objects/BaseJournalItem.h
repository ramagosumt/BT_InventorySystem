#pragma once

#include "CoreMinimal.h"
#include "Data/BaseItemData.h"
#include "BaseJournalItem.generated.h"

class UListInventoryComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UBaseJournalItem : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseJournalItem();

	FORCEINLINE UListInventoryComponent* GetOwningInventory() const { return OwningInventory; }
	FORCEINLINE FName GetItemID() const { return ID; }
	FORCEINLINE EItemType GetItemType() const { return Type; }
	FORCEINLINE FItemTextData GetItemTextData() { return TextData; }
	FORCEINLINE FItemAssetData GetItemAssetData() const { return AssetData; }
	
	FORCEINLINE void SetOwningInventory(UListInventoryComponent* NewListInventory) { OwningInventory = NewListInventory; }
	FORCEINLINE void SetItemID(const FName NewID) { ID = NewID; }
	FORCEINLINE void SetItemType(const EItemType NewType) { Type = NewType; }
	FORCEINLINE void SetItemTextData(const FItemTextData& NewTextData) { TextData = NewTextData; }
	FORCEINLINE void SetItemAssetData(const FItemAssetData& NewAssetData) { AssetData = NewAssetData; }

protected:
	UPROPERTY()
	UListInventoryComponent* OwningInventory;

	FName ID;
	EItemType Type;
	FItemTextData TextData;
	FItemAssetData AssetData;

	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
