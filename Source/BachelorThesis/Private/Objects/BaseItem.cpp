#include "Objects/BaseItem.h"
#include "Components/GridInventoryComponent.h"

UBaseItem::UBaseItem() :
OwningInventory(nullptr), Quantity(0), Type(EItemType::Common), bIsRotated(false)
{
}

void UBaseItem::SetItemQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
	}

	if (OwningInventory)
	{
		if (Quantity <= 0)
		{
			OwningInventory->RemoveSingleInstanceOfItem(this);
		}
	}
}

FIntPoint UBaseItem::GetItemDimensions() const
{
	if (bIsRotated) return FIntPoint(GetItemAssetData().Dimensions.Y, GetItemAssetData().Dimensions.X);
	return GetItemAssetData().Dimensions;
}
