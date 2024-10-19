#include "Actors/BasePickupItem.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/BaseInteractableComponent.h"
#include "Components/GridInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Objects/BaseItem.h"
#include "UIs/InteractionButtonWidget.h"

ABasePickupItem::ABasePickupItem()
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

void ABasePickupItem::OnConstruction(const FTransform& Transform)
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

void ABasePickupItem::BeginPlay()
{
	Super::BeginPlay();

	InitializeItem(UBaseItem::StaticClass(), Quantity);
}

void ABasePickupItem::BeginFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(true);
}

void ABasePickupItem::EndFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(false);
}

void ABasePickupItem::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		ObtainedBy(PlayerCharacter);
	}
}

void ABasePickupItem::InitializeItem(const TSubclassOf<UBaseItem>& BaseClass, const int32 NewQuantity)
{
	if (InventoryDataTable && !InventoryID.IsNone())
	{
		const FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(InventoryID, InventoryID.ToString());

		ItemReference = NewObject<UBaseItem>(this, BaseClass);

		ItemReference->SetItemID(ItemData->ID);
		ItemReference->SetItemType(ItemData->Type);
		ItemReference->SetItemTextData(ItemData->TextData);
		ItemReference->SetItemNumericData(ItemData->NumericData);
		ItemReference->SetItemAssetData(ItemData->AssetData);

		Quantity <= 0 ? ItemReference->SetItemQuantity(1) : ItemReference->SetItemQuantity(NewQuantity);

		Mesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractionData();
	}
}

void ABasePickupItem::UpdateInteractionData()
{
	ItemInteractionData.ItemInteractionMode = EInteractionMode::InventoryItem;
	ItemInteractionData.Action = ItemReference->GetItemTextData().InteractionText;
	ItemInteractionData.Name = ItemReference->GetItemTextData().Name;
	ItemInteractionData.Quantity = ItemReference->GetItemQuantity();
	ItemInteractionData.Duration = InteractionDuration;
	
	InteractionData = ItemInteractionData;

	if (BaseInteractable) BaseInteractable->SetInteractionDuration(ItemInteractionData.Duration);
}

void ABasePickupItem::ObtainedBy(const ABachelorThesisCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UGridInventoryComponent* GridInventory = Taker->GetGridInventory())
			{
				GridInventory->HandleAddItem(ItemReference);
				Destroy();
			}
		}
	}
}

#if WITH_EDITOR
void ABasePickupItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ABasePickupItem, InventoryID))
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
