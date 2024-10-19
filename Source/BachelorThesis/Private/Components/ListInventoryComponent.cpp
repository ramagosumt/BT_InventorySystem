#include "Components/ListInventoryComponent.h"
#include "Objects/BaseJournalItem.h"

UListInventoryComponent::UListInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UListInventoryComponent::AddItem(UBaseJournalItem* ItemToAdd)
{
	if (IsValid(ItemToAdd))
	{
		ItemToAdd->SetOwningInventory(this);
		
		InventoryContent.Add(ItemToAdd);
		SortAlphabet_Ascend();
		
		OnInventoryUpdated.Broadcast();
	}
}

void UListInventoryComponent::RemoveItem(UBaseJournalItem* ItemToRemove)
{
	if (IsValid(ItemToRemove))
	{
		InventoryContent.RemoveSingle(ItemToRemove);
		OnInventoryUpdated.Broadcast();
	}
}

void UListInventoryComponent::SortAlphabet_Ascend()
{
	InventoryContent.Sort([](UBaseJournalItem& JournalItemA, UBaseJournalItem& JournalItemB)
		{
			return JournalItemA.GetItemTextData().Name.CompareTo(JournalItemB.GetItemTextData().Name) < 0;
		}
	);
}

void UListInventoryComponent::SortAlphabet_Descend()
{
	InventoryContent.Sort([](UBaseJournalItem& JournalItemA,UBaseJournalItem& JournalItemB)
		{
			return JournalItemA.GetItemTextData().Name.CompareTo(JournalItemB.GetItemTextData().Name) > 0;
		}
	);
}