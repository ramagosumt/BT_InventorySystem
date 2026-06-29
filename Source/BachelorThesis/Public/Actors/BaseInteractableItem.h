#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BaseInteractableItem.generated.h"

#define ECC_InteractableItem ECC_GameTraceChannel1

class USphereComponent;
class UBaseInteractableComponent;
class UWidgetComponent;

UCLASS()
class BACHELORTHESIS_API ABaseInteractableItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ABaseInteractableItem();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USphereComponent* OverlapRadius;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UBaseInteractableComponent* BaseInteractable;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData ItemInteractionData;
	
	UPROPERTY(EditDefaultsOnly, Category= "Interaction")
	FText InteractionAction;

	UPROPERTY(EditDefaultsOnly, Category= "Interaction")
	FText InteractionName;
	
	UPROPERTY(EditDefaultsOnly, Category= "Interaction")
	float InteractionDuration;
	
	virtual void UpdateInteractionData();
};
