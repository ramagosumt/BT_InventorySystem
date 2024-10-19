#include "Actors/BasePickupQuestItem.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/SlotInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BaseInteractableComponent.h"
#include "Components/WidgetComponent.h"
#include "Objects/BaseQuestItem.h"
#include "UIs/InteractionButtonWidget.h"

ABasePickupQuestItem::ABasePickupQuestItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_InventoryItem);
	
	OverlapRadius = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapRadius"));
	OverlapRadius->SetupAttachment(Mesh);
	OverlapRadius->InitSphereRadius(325.f);

	BaseInteractable = CreateDefaultSubobject<UBaseInteractableComponent>(TEXT("BaseInteractable"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetClass(ConstructorHelpers::FClassFinder<UInteractionButtonWidget> (TEXT("/Game/Rama_InteractionSystem/UIs/W_InteractionButton")).Class);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetHiddenInGame(true);
}

void ABasePickupQuestItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (InventoryDataTable)
	{
		if (const FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(InventoryID, InventoryID.ToString()))
		{
			Mesh->SetStaticMesh(ItemData->AssetData.Mesh);
		}
	}
}

void ABasePickupQuestItem::BeginPlay()
{
	Super::BeginPlay();

	InitializeItem(UBaseQuestItem::StaticClass());
}

void ABasePickupQuestItem::BeginFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(true);
}

void ABasePickupQuestItem::EndFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(false);
}

void ABasePickupQuestItem::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		ObtainedBy(PlayerCharacter);
	}
}

void ABasePickupQuestItem::InitializeItem(const TSubclassOf<UBaseQuestItem>& BaseClass)
{
	if (InventoryDataTable && !InventoryID.IsNone())
	{
		const FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(InventoryID, InventoryID.ToString());

		ItemReference = NewObject<UBaseQuestItem>(this, BaseClass);

		ItemReference->SetItemID(ItemData->ID);
		ItemReference->SetItemType(ItemData->Type);
		ItemReference->SetItemTextData(ItemData->TextData);
		ItemReference->SetItemAssetData(ItemData->AssetData);

		Mesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractionData();
	}
}

void ABasePickupQuestItem::UpdateInteractionData()
{
	ItemInteractionData.ItemInteractionMode = EInteractionMode::InventoryItem;
	ItemInteractionData.Action = ItemReference->GetItemTextData().InteractionText;
	ItemInteractionData.Name = ItemReference->GetItemTextData().Name;
	ItemInteractionData.Duration = InteractionDuration;
	
	InteractionData = ItemInteractionData;

	if (BaseInteractable) BaseInteractable->SetInteractionDuration(ItemInteractionData.Duration);
}

void ABasePickupQuestItem::ObtainedBy(const ABachelorThesisCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (USlotInventoryComponent* SlotInventory = Taker->GetSlotInventory())
			{
				SlotInventory->AddItem(ItemReference);
				Destroy();
			}
		}
	}
}

#if WITH_EDITOR
void ABasePickupQuestItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ABasePickupQuestItem, InventoryID))
	{
		if (InventoryDataTable)
		{
			if (const FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(InventoryID, InventoryID.ToString()))
			{
				Mesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
#endif
