#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceInteractionComponent.generated.h"

#define ECC_InventoryItem ECC_GameTraceChannel1
#define ECC_NPC ECC_GameTraceChannel2

class UInteractionButtonWidget;
class IInteractionInterface;
class UBaseInteractableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UTraceInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly)
	AActor* ActorInFocused;
	
	UPROPERTY()
	UBaseInteractableComponent* ActorInteractableComponent;

	UPROPERTY()
	UInteractionButtonWidget* ActorInteractionWidget;

	float TraceDistance;
	float InteractionDuration;
	float InteractionPercentage;
	float InteractionInterval;
	
	bool bIsCurrentlyTracing;

	FTimerHandle InteractionTimer;

public:	
	UTraceInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleInteraction(const bool IsInteracting);
	void ResetInteraction();

	FORCEINLINE void SetTraceDistance(const float NewDistance) { TraceDistance = NewDistance; }

protected:
	void CheckInteraction();
	void FoundInteraction();
	void FoundNoInteraction();
	void StartInteraction();
	void StopInteraction();

	UFUNCTION()
	void UpdateInteractionPercentage();
	
};
