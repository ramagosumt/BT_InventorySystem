#include "Actors/BasePickupJournalItem.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/ListInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BaseInteractableComponent.h"
#include "Components/BoxComponent.h"
#include "Components/GDC_JournalItem.h"
#include "Components/WidgetComponent.h"
#include "Objects/BaseJournalItem.h"
#include "UIs/InteractionButtonWidget.h"

ABasePickupJournalItem::ABasePickupJournalItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_InventoryItem);
	
	OverlapRadius = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapRadius"));
	OverlapRadius->SetupAttachment(Mesh);
	OverlapRadius->InitSphereRadius(325.f);

	BaseInteractable = CreateDefaultSubobject<UBaseInteractableComponent>(TEXT("BaseInteractable"));
	BaseInteractable->SetIsInBoxNeeded(true);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetClass(ConstructorHelpers::FClassFinder<UInteractionButtonWidget> (TEXT("/Game/Rama_InteractionSystem/UIs/W_InteractionButton")).Class);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetHiddenInGame(true);

	InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
	InteractionArea->SetupAttachment(GetRootComponent());
	InteractionArea->ComponentTags.Add("InteractionArea");

	DialogueComponent = CreateDefaultSubobject<UGDC_JournalItem>(TEXT("DialogueComponent"));
	DialogueComponent->OnDialogueEnd.AddUniqueDynamic(this, &ABasePickupJournalItem::OnDialogueEnd);
}

void ABasePickupJournalItem::OnConstruction(const FTransform& Transform)
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

void ABasePickupJournalItem::BeginPlay()
{
	Super::BeginPlay();

	InitializeItem(UBaseJournalItem::StaticClass());
}

void ABasePickupJournalItem::BeginFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(true);
}

void ABasePickupJournalItem::EndFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(false);
}

void ABasePickupJournalItem::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		DialogueComponent->OpenConversation();
		WidgetComponent->SetHiddenInGame(true);
		CurrentInteractingActor = PlayerCharacter;
	}
}

void ABasePickupJournalItem::InitializeItem(const TSubclassOf<UBaseJournalItem>& BaseClass)
{
	if (InventoryDataTable && !InventoryID.IsNone())
	{
		const FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(InventoryID, InventoryID.ToString());

		ItemReference = NewObject<UBaseJournalItem>(this, BaseClass);

		ItemReference->SetItemID(ItemData->ID);
		ItemReference->SetItemType(ItemData->Type);
		ItemReference->SetItemTextData(ItemData->TextData);
		ItemReference->SetItemAssetData(ItemData->AssetData);

		Mesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractionData();
	}
}

void ABasePickupJournalItem::UpdateInteractionData()
{
	ItemInteractionData.ItemInteractionMode = EInteractionMode::InventoryItem;
	ItemInteractionData.Action = ItemReference->GetItemTextData().InteractionText;
	ItemInteractionData.Name = ItemReference->GetItemTextData().Name;
	ItemInteractionData.Duration = InteractionDuration;
	
	InteractionData = ItemInteractionData;
	
	if (BaseInteractable) BaseInteractable->SetInteractionDuration(ItemInteractionData.Duration);
}

void ABasePickupJournalItem::ObtainedBy(const ABachelorThesisCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UListInventoryComponent* ListInventory = Taker->GetListInventory())
			{
				ListInventory->AddItem(ItemReference);
				Destroy();
			}
		}
	}
}

void ABasePickupJournalItem::OnDialogueEnd()
{
	if (CurrentInteractingActor) ObtainedBy(CurrentInteractingActor);
}

#if WITH_EDITOR
void ABasePickupJournalItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ABasePickupJournalItem, InventoryID))
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
