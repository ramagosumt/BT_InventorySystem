#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BaseInteractableItem.generated.h"

#define ECC_InteractableItem ECC_GameTraceChannel1

class USphereComponent;
class UWidgetComponent;
class UBaseInteractableComponent;

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
	
private:
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USphereComponent* OverlapRadius;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UBaseInteractableComponent* BaseInteractable;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, Category= "Defaults")
	float InteractionDuration;
	
	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData ItemInteractionData;
	
	void UpdateInteractionData();
};
