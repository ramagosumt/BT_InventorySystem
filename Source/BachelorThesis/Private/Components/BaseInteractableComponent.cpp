#include "Components/BaseInteractableComponent.h"
#include "Characters/BachelorThesisCharacter.h"
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
		if (IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(GetOwner())) 
			return InteractableActor->InteractionData;
	
	return FInteractionData();
}

void UBaseInteractableComponent::BeginFocus()
{
	if (GetOwner())
		if (IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(GetOwner()))
		{
			InteractableActor->BeginFocus();
			if (const auto Widget = Cast<UWidgetComponent>(GetOwner()->GetComponentByClass(UWidgetComponent::StaticClass())))
				Widget->SetHiddenInGame(false);
		}
}

void UBaseInteractableComponent::EndFocus()
{
	if (GetOwner())
		if (IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(GetOwner()))
		{
			InteractableActor->EndFocus();
			if (const auto Widget = Cast<UWidgetComponent>(GetOwner()->GetComponentByClass(UWidgetComponent::StaticClass())))
				Widget->SetHiddenInGame(true);
		}
}

void UBaseInteractableComponent::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (GetOwner()) 
		if (IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(GetOwner())) 
			InteractableActor->Interact(PlayerCharacter);
}

void UBaseInteractableComponent::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Character = Cast<ABachelorThesisCharacter>(OtherActor)) Character->ResetInteract();
}

void UBaseInteractableComponent::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto Character = Cast<ABachelorThesisCharacter>(OtherActor)) Character->ResetInteract();
}
