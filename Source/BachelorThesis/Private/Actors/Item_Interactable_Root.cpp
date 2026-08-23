#include "Actors/Item_Interactable_Root.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BaseInteractableComponent.h"
#include "UIs/InteractionButtonWidget.h"

AItem_Interactable_Root::AItem_Interactable_Root()
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

void AItem_Interactable_Root::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateInteractionData();
}

void AItem_Interactable_Root::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_Interactable_Root::BeginFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(true);
}

void AItem_Interactable_Root::EndFocus()
{
	IInteractionInterface::EndFocus();
	
	if (Mesh) Mesh->SetRenderCustomDepth(false);
}

void AItem_Interactable_Root::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);	
	
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting with PlayerCharacter"));
	}
}

void AItem_Interactable_Root::UpdateInteractionData()
{
	ItemInteractionData.ItemInteractionMode = EInteractionMode::Item_Interactable;
	ItemInteractionData.Action = InteractionAction;
	ItemInteractionData.Name = InteractionName;
	ItemInteractionData.Duration = InteractionDuration;
	
	InteractionData = ItemInteractionData;

	if (BaseInteractable) BaseInteractable->SetInteractionDuration(ItemInteractionData.Duration);
}
