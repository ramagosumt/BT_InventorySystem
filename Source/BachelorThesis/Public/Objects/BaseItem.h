#pragma once

#include "CoreMinimal.h"
#include "Data/BaseItemData.h"
#include "BaseItem.generated.h"

class UGridInventoryComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseItem();

	FORCEINLINE UGridInventoryComponent* GetOwningInventory() const { return OwningInventory; }
	FORCEINLINE FName GetItemID() const { return ID; }
	FORCEINLINE int32 GetItemQuantity() const { return Quantity; }
	FORCEINLINE EItemType GetItemType() const { return Type; }
	FORCEINLINE FItemTextData GetItemTextData() { return TextData; }
	FORCEINLINE FItemNumericData GetItemNumericData() const { return NumericData; }
	FORCEINLINE FItemAssetData GetItemAssetData() const { return AssetData; }
	FIntPoint GetItemDimensions() const;
	FORCEINLINE bool GetItemRotated() const { return bIsRotated; }
	
	FORCEINLINE void SetOwningInventory(UGridInventoryComponent* NewGridInventory) { OwningInventory = NewGridInventory; }
	FORCEINLINE void SetItemID(const FName NewID) { ID = NewID; }
	void SetItemQuantity(const int32 NewQuantity);
	FORCEINLINE void SetItemType(const EItemType NewType) { Type = NewType; }
	FORCEINLINE void SetItemTextData(const FItemTextData& NewTextData) { TextData = NewTextData; }
	FORCEINLINE void SetItemNumericData(const FItemNumericData& NewNumericData) { NumericData = NewNumericData; }
	FORCEINLINE void SetItemAssetData(const FItemAssetData& NewAssetData) { AssetData = NewAssetData; }
	FORCEINLINE void SetItemRotated() { bIsRotated = !bIsRotated; }
	
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

protected:
	UPROPERTY()
	UGridInventoryComponent* OwningInventory;

	FName ID;
	int32 Quantity;
	EItemType Type;
	FItemTextData TextData;
	FItemNumericData NumericData;
	FItemAssetData AssetData;
	bool bIsRotated;

	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
