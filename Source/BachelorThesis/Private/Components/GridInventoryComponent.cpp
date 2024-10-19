#include "BachelorThesis/Public/Components/GridInventoryComponent.h"

UGridInventoryComponent::UGridInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGridInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < GetSizeX() * GetSizeY(); i++) InventoryContent.Add(nullptr);
}

TMap<UBaseItem*, FTile> UGridInventoryComponent::GetAllItems()
{
	TMap<UBaseItem*, FTile> Items;

	int32 ItemIndex = 0;
	for (const auto Item : InventoryContent)
	{
		if (IsValid(Item))
		{
			if (!Items.Contains(Item)) Items.Add(Item, IndexToTile(ItemIndex));
		}

		ItemIndex++;
	}
	
	return Items;
}

UBaseItem* UGridInventoryComponent::GetItemAtIndex(const int32 Index) const
{
	if (InventoryContent.IsValidIndex(Index)) if (IsValid(InventoryContent[Index])) return InventoryContent[Index];
	
	return nullptr;
}

UBaseItem* UGridInventoryComponent::FindNextPartialStack(UBaseItem* InItem) const
{
	if (const TArray<UBaseItem*>::ElementType* Result = InventoryContent.FindByPredicate([&InItem](const UBaseItem* Item)
		{
			return Item != nullptr && Item->GetItemID() == InItem->GetItemID() && ! Item->IsFullItemStack();
		}
		))
	{
		return *Result;
	}

	return nullptr;
}

void UGridInventoryComponent::RemoveSingleInstanceOfItem(const UBaseItem* OutItem)
{
	if (OutItem)
	{
		int32 ItemIndex = 0;
		for (const auto Item : InventoryContent)
		{
			if (Item == OutItem) InventoryContent[ItemIndex] = nullptr;
			ItemIndex++;
		}
		
		OnInventoryUpdated.Broadcast();
	}
}

FItemAddResult UGridInventoryComponent::HandleAddItem(UBaseItem* InItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InItem->GetItemQuantity();
		if (!InItem->GetItemNumericData().bIsStackable)
		{
			return HandleNonStackableItems(InItem, InitialRequestedAddAmount);
		}

		const int32 StackableAmountAdded = HandleStackableItems(InItem, InitialRequestedAddAmount);
		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount);
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded);
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone();
		}
	}

	check(false);
	return FItemAddResult::AddedNone();
}

FItemAddResult UGridInventoryComponent::HandleNonStackableItems(UBaseItem* InItem, const int32 AmountToAdd)
{
	if (IsValid(InItem))
	{
		int32 ItemIndex = 0;
		for (const auto Item : InventoryContent)
		{
			if (IsSpaceAvailable(InItem, ItemIndex))
			{
				AddNewItem(InItem, ItemIndex, AmountToAdd);
				return FItemAddResult::AddedAll(AmountToAdd);
			}
			ItemIndex++;
		}

		InItem->SetItemRotated();
		ItemIndex = 0;
		for (const auto Item : InventoryContent)
		{
			if (IsSpaceAvailable(InItem, ItemIndex))
			{
				AddNewItem(InItem, ItemIndex, AmountToAdd);
				return FItemAddResult::AddedAll(AmountToAdd);
			}
			ItemIndex++;
		}

		InItem->SetItemRotated();
	}

	return FItemAddResult::AddedNone();
}

int32 UGridInventoryComponent::HandleStackableItems(UBaseItem* InItem, const int32 AmountToAdd)
{
	if (AmountToAdd <= 0) return 0;

	int32 AmountToDistribute = AmountToAdd;

	UBaseItem* ExistingItem = FindNextPartialStack(InItem);
	while (ExistingItem)
	{
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItem, AmountToDistribute);
		ExistingItem->SetItemQuantity(ExistingItem->GetItemQuantity() + AmountToMakeFullStack);
		
		AmountToDistribute -= AmountToMakeFullStack;
		InItem->SetItemQuantity(AmountToDistribute);

		if (AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return AmountToAdd;
		}
		
		ExistingItem = FindNextPartialStack(InItem);
	}

	int32 ItemIndex = 0;
	for (const auto Item : InventoryContent)
	{
		if (IsSpaceAvailable(InItem, ItemIndex))
		{
			AddNewItem(InItem, ItemIndex, AmountToDistribute);
			return AmountToAdd;
		}
		ItemIndex++;
	}

	InItem->SetItemRotated();
	ItemIndex = 0;
	for (const auto Item : InventoryContent)
	{
		if (IsSpaceAvailable(InItem, ItemIndex))
		{
			AddNewItem(InItem, ItemIndex, AmountToAdd);
			return AmountToAdd;
		}
		ItemIndex++;
	}

	InItem->SetItemRotated();

	OnInventoryUpdated.Broadcast();
	return AmountToAdd - AmountToDistribute;
}

bool UGridInventoryComponent::IsSpaceAvailable(const UBaseItem* InItem, const int32 TopLeftIndex) const
{
	const int32 TileX = IndexToTile(TopLeftIndex).X;
	const int32 TileY = IndexToTile(TopLeftIndex).Y;
	const int32 ItemDimensionX = InItem->GetItemDimensions().X;
	const int32 ItemDimensionY = InItem->GetItemDimensions().Y;

	for (int32 XIndex = TileX; XIndex < TileX + ItemDimensionX; XIndex++)
	{
		for (int32 YIndex = TileY; YIndex < TileY + ItemDimensionY; YIndex++)
		{
			if (XIndex >= 0 && YIndex >= 0 && XIndex < GetSizeX() && YIndex < GetSizeY())
			{
				if (GetItemAtIndex(TileToIndex(FTile(XIndex, YIndex)))) return false;
			}
			else return false;
		}
	}

	return true;
}

void UGridInventoryComponent::AddNewItem(UBaseItem* InItem, const int32 TopLeftIndex, const int32 AmountToAdd)
{
	InItem->SetOwningInventory(this);
	InItem->SetItemQuantity(AmountToAdd);
	
	const int32 TileX = IndexToTile(TopLeftIndex).X;
	const int32 TileY = IndexToTile(TopLeftIndex).Y;
	const int32 ItemDimensionX = InItem->GetItemDimensions().X;
	const int32 ItemDimensionY = InItem->GetItemDimensions().Y;

	for (int32 XIndex = TileX; XIndex < TileX + ItemDimensionX; XIndex++)
	{
		for (int32 YIndex = TileY; YIndex < TileY + ItemDimensionY; YIndex++)
		{
			InventoryContent[TileToIndex(FTile(XIndex, YIndex))] = InItem;
		}
	}

	OnInventoryUpdated.Broadcast();
}

int32 UGridInventoryComponent::CalculateNumberForFullStack(const UBaseItem* InStackableItem, const int32 InitialAmountToAdd)
{
	const int AddAmountToMakeFullStack = InStackableItem->GetItemNumericData().MaxStackSize - InStackableItem->GetItemQuantity();

	return FMath::Min(InitialAmountToAdd, AddAmountToMakeFullStack);
}