#include "Components/BaseInteractableComponent.h"
#include "Actors/BasePickupItem.h"
#include "Actors/BasePickupJournalItem.h"
#include "Actors/BasePickupQuestItem.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Characters/BaseNPC.h"
#include "Components/ShapeComponent.h"
#include "Components/WidgetComponent.h"

UBaseInteractableComponent::UBaseInteractableComponent() :
	InteractionArea(nullptr), InteractionDuration(0.f)
{
}

void UBaseInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), FName(TEXT("InteractionArea"))).IsValidIndex(0))
	{
		InteractionArea = Cast<UShapeComponent>(GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), FName(TEXT("InteractionArea")))[0]);
		if (InteractionArea)
		{
			InteractionArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &UBaseInteractableComponent::OnInteractionBeginOverlap);
			InteractionArea->OnComponentEndOverlap.AddUniqueDynamic(this, &UBaseInteractableComponent::OnInteractionEndOverlap);
		}
	}
}

bool UBaseInteractableComponent::IsFocusable(const AActor* ActorToFocus) const
{
	if (IsInBoxNeeded) if (InteractionArea) return InteractionArea->IsOverlappingActor(ActorToFocus);
	return true;
}

FInteractionData UBaseInteractableComponent::GetOwnerInteractionData() const
{
	if (GetOwner())
	{
		ABasePickupItem* BasePickupItem = Cast<ABasePickupItem>(GetOwner());
		ABasePickupQuestItem* BasePickupQuestItem = Cast<ABasePickupQuestItem>(GetOwner());
		ABasePickupJournalItem* BasePickupJournalItem = Cast<ABasePickupJournalItem>(GetOwner());
		ABaseNPC* BaseNPC = Cast<ABaseNPC>(GetOwner());

		if (BasePickupItem) return BasePickupItem->InteractionData;
		if (BasePickupQuestItem) return BasePickupQuestItem->InteractionData;
		if (BasePickupJournalItem) return BasePickupJournalItem->InteractionData;
		if (BaseNPC) return BaseNPC->InteractionData;
	}

	return FInteractionData();
}

void UBaseInteractableComponent::BeginFocus()
{
	IInteractionInterface::BeginFocus();

	if (GetOwner())
	{
		ABasePickupItem* BasePickupItem = Cast<ABasePickupItem>(GetOwner());
		ABasePickupQuestItem* BasePickupQuestItem = Cast<ABasePickupQuestItem>(GetOwner());
		ABasePickupJournalItem* BasePickupJournalItem = Cast<ABasePickupJournalItem>(GetOwner());
		ABaseNPC* BaseNPC = Cast<ABaseNPC>(GetOwner());

		if (BasePickupItem)
		{
			BasePickupItem->BeginFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(false);
		}
		
		else if (BasePickupQuestItem)
		{
			BasePickupQuestItem->BeginFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupQuestItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(false);
		}
		
		else if (BasePickupJournalItem)
		{
			BasePickupJournalItem->BeginFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupJournalItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(false);
		}

		else if (BaseNPC)
		{
			BaseNPC->BeginFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BaseNPC->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(false);
		}
	}
}

void UBaseInteractableComponent::EndFocus()
{
	IInteractionInterface::BeginFocus();
	
	if (GetOwner())
	{
		ABasePickupItem* BasePickupItem = Cast<ABasePickupItem>(GetOwner());
		ABasePickupQuestItem* BasePickupQuestItem = Cast<ABasePickupQuestItem>(GetOwner());
		ABasePickupJournalItem* BasePickupJournalItem = Cast<ABasePickupJournalItem>(GetOwner());
		ABaseNPC* BaseNPC = Cast<ABaseNPC>(GetOwner());

		if (BasePickupItem)
		{
			BasePickupItem->EndFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(true);
		}
		
		else if (BasePickupQuestItem)
		{
			BasePickupQuestItem->EndFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupQuestItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(true);
		}
		
		else if (BasePickupJournalItem)
		{
			BasePickupJournalItem->EndFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BasePickupJournalItem->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(true);
		}

		else if (BaseNPC)
		{
			BaseNPC->EndFocus();
			if (const auto Widget = Cast<UWidgetComponent>(BaseNPC->GetComponentByClass(UWidgetComponent::StaticClass()))) Widget->SetHiddenInGame(true);
		}
	}
}

void UBaseInteractableComponent::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (GetOwner())
	{
		ABasePickupItem* BasePickupItem = Cast<ABasePickupItem>(GetOwner());
		ABasePickupQuestItem* BasePickupQuestItem = Cast<ABasePickupQuestItem>(GetOwner());
		ABasePickupJournalItem* BasePickupJournalItem = Cast<ABasePickupJournalItem>(GetOwner());
		ABaseNPC* BaseNPC = Cast<ABaseNPC>(GetOwner());

		if (BasePickupItem) BasePickupItem->Interact(PlayerCharacter);
		else if (BasePickupQuestItem) BasePickupQuestItem->Interact(PlayerCharacter);
		else if (BasePickupJournalItem) BasePickupJournalItem->Interact(PlayerCharacter);
		else if (BaseNPC) BaseNPC->Interact(PlayerCharacter);
	}
}

void UBaseInteractableComponent::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Character = Cast<ABachelorThesisCharacter>(OtherActor)) Character->ResetInteract();
}

void UBaseInteractableComponent::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto Character = Cast<ABachelorThesisCharacter>(OtherActor)) Character->ResetInteract();
}
