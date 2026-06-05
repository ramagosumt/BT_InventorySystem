#include "Actors/BaseInteractableItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BaseInteractableComponent.h"
#include "UIs/InteractionButtonWidget.h"

ABaseInteractableItem::ABaseInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_InteractableItem);
	
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

void ABaseInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateInteractionData();
}

void ABaseInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInteractableItem::BeginFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(true);
}

void ABaseInteractableItem::EndFocus()
{
	IInteractionInterface::EndFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(false);
}

void ABaseInteractableItem::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);
	
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting with PlayerCharacter"));
	}
}

void ABaseInteractableItem::UpdateInteractionData()
{
	ItemInteractionData.ItemInteractionMode = EInteractionMode::InteractableItem;
	ItemInteractionData.Action = FText::FromString("interact with");
	ItemInteractionData.Name = FText::FromString("item");
	ItemInteractionData.Duration = InteractionDuration;
	
	InteractionData = ItemInteractionData;

	if (BaseInteractable) BaseInteractable->SetInteractionDuration(ItemInteractionData.Duration);
}
