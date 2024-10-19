#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "BaseInteractableComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UBaseInteractableComponent : public UActorComponent, public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY()
	UShapeComponent* InteractionArea;

	float InteractionDuration;

	bool IsInBoxNeeded;

public:	
	UBaseInteractableComponent();

	bool IsFocusable(const AActor* ActorToFocus) const;
	FInteractionData GetOwnerInteractionData() const;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;

	FORCEINLINE float GetInteractionDuration() const { return InteractionDuration; }
	FORCEINLINE bool GetIsInBoxNeeded() const { return IsInBoxNeeded; }
	
	FORCEINLINE void SetInteractionDuration(const float NewDuration) { InteractionDuration = NewDuration; }
	FORCEINLINE void SetIsInBoxNeeded(const bool NewCondition) { IsInBoxNeeded = NewCondition; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
