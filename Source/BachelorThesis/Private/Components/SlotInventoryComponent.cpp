#include "Components/SlotInventoryComponent.h"

USlotInventoryComponent::USlotInventoryComponent() :
	InventorySize(0)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USlotInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < GetSize(); i++) InventoryContent.Add(nullptr);
}

void USlotInventoryComponent::AddItem(UBaseQuestItem* ItemToAdd)
{
	if (IsValid(ItemToAdd))
	{
		ItemToAdd->SetOwningInventory(this);

		int32 CurrentIndex = 0;
		for (const auto Item : InventoryContent) { if (IsValid(Item)) CurrentIndex++; }
		
		InventoryContent[CurrentIndex] = ItemToAdd;
		
		OnInventoryUpdated.Broadcast();
	}
}

void USlotInventoryComponent::RemoveItem(UBaseQuestItem* ItemToRemove)
{
	if (IsValid(ItemToRemove))
	{
		InventoryContent.RemoveSingle(ItemToRemove);
		OnInventoryUpdated.Broadcast();
	}
}
