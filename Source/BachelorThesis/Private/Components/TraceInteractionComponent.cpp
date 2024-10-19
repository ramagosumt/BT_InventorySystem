#include "Components/TraceInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/BaseInteractableComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/InteractionInterface.h"
#include "UIs/InteractionButtonWidget.h"
#include "Frameworks/MainHUD.h"

UTraceInteractionComponent::UTraceInteractionComponent() :
	ActorInFocused(nullptr), ActorInteractableComponent(nullptr), ActorInteractionWidget(nullptr),
	TraceDistance(0.f), InteractionDuration(0.f), InteractionPercentage(0.f), InteractionInterval(0.01f), bIsCurrentlyTracing(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckInteraction();
}

void UTraceInteractionComponent::CheckInteraction()
{
	if (const auto PlayerCamera = Cast<ABachelorThesisCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetFPCamera())
	{
		const FVector TraceStart = PlayerCamera->GetComponentLocation();
		const FVector TraceEnd = TraceStart + UKismetMathLibrary::GetForwardVector(PlayerCamera->GetComponentRotation()) * TraceDistance;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_InventoryItem));
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_NPC));

		const TArray<AActor*, FDefaultAllocator> IgnoreActors;
		FHitResult TraceHit;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, 0.f,
			ObjectTypesArray, false, IgnoreActors, EDrawDebugTrace::None, TraceHit,
			true, FLinearColor::Red, FLinearColor::Green, 5.f))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor()->GetComponentByClass(UBaseInteractableComponent::StaticClass()))
				{
					if (TraceHit.GetActor() != ActorInFocused)
					{
						ActorInFocused = TraceHit.GetActor();
						FoundInteraction();

						return;
					}

					if (TraceHit.GetActor() == ActorInFocused) return;
				}
			}
		}

		FoundNoInteraction();
	}
}

void UTraceInteractionComponent::FoundInteraction()
{
	if (bIsCurrentlyTracing) if (ActorInteractableComponent) ActorInteractableComponent->EndFocus();
	
	ActorInteractableComponent = Cast<UBaseInteractableComponent>(ActorInFocused->GetComponentByClass(UBaseInteractableComponent::StaticClass()));
	if (ActorInteractableComponent->IsFocusable(GetOwner()))
	{
		if (const auto HUD = Cast<ABachelorThesisCharacter>(GetOwner())->GetMainHUD())
		{
			HUD->UpdateInteractionBar(ActorInteractableComponent->GetOwnerInteractionData());
		}
		
		ActorInteractableComponent->BeginFocus();
	
		bIsCurrentlyTracing = true;
	}
	else FoundNoInteraction();
}

void UTraceInteractionComponent::FoundNoInteraction()
{
	if (bIsCurrentlyTracing)
	{
		if (ActorInFocused) if (ActorInteractableComponent) ActorInteractableComponent->EndFocus();

		if (const auto HUD = Cast<ABachelorThesisCharacter>(GetOwner())->GetMainHUD()) HUD->HideInteractionBar();
		
		ActorInFocused = nullptr;
		ActorInteractableComponent = nullptr;

		bIsCurrentlyTracing = false;

		StopInteraction();
	}
}

void UTraceInteractionComponent::HandleInteraction(const bool IsInteracting)
{
	if (IsInteracting)
	{
		if (bIsCurrentlyTracing)
		{
			InteractionDuration = ActorInteractableComponent->GetInteractionDuration();
			if (InteractionDuration > 0.f)
			{
				const UWidgetComponent* ActorWidgetComponent = Cast<UWidgetComponent>(ActorInteractableComponent->GetOwner()->GetComponentByClass(UWidgetComponent::StaticClass()));
				ActorInteractionWidget = Cast<UInteractionButtonWidget>(ActorWidgetComponent->GetWidget());

				FTimerDynamicDelegate TimerDynamicDelegate;
				TimerDynamicDelegate.BindUFunction(this, FName(TEXT("UpdateInteractionPercentage")));
				InteractionTimer = UKismetSystemLibrary::K2_SetTimerDelegate(TimerDynamicDelegate, InteractionInterval, true);
			}
			else
			{
				StartInteraction();
			}
		}
	}
	else StopInteraction();
}

void UTraceInteractionComponent::ResetInteraction()
{
	if (bIsCurrentlyTracing) if (ActorInFocused) if (ActorInteractableComponent) ActorInteractableComponent->EndFocus();

	if (const auto HUD = Cast<ABachelorThesisCharacter>(GetOwner())->GetMainHUD()) HUD->HideInteractionBar();
	
	ActorInFocused = nullptr;
	ActorInteractableComponent = nullptr;
	
	bIsCurrentlyTracing = false;
	
	StopInteraction();
}

void UTraceInteractionComponent::StartInteraction()
{
	if (GetOwner())
	{
		ActorInteractableComponent->Interact(Cast<ABachelorThesisCharacter>(GetOwner()));
	}
}

void UTraceInteractionComponent::StopInteraction()
{
	if (InteractionPercentage > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractionTimer);
		
		if (ActorInteractionWidget) ActorInteractionWidget->SetInteractionPercent(0.f);
		
		InteractionPercentage = 0.f;
		
		ActorInteractionWidget = nullptr;
	}
}

void UTraceInteractionComponent::UpdateInteractionPercentage()
{
	UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), InteractionPercentage, InteractionDuration, InteractionInterval);
	InteractionPercentage = InteractionPercentage + 1/InteractionDuration * InteractionInterval;
	if (ActorInteractionWidget) ActorInteractionWidget->SetInteractionPercent(InteractionPercentage);

	if (InteractionPercentage > 1)
	{
		StopInteraction();
		StartInteraction();
	}
}
