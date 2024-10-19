#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BasePickupQuestItem.generated.h"

class UBaseInteractableComponent;
class USphereComponent;
class UWidgetComponent;
class UDataTable;
class UBaseQuestItem;

#define ECC_InventoryItem ECC_GameTraceChannel1

UCLASS()
class BACHELORTHESIS_API ABasePickupQuestItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USphereComponent* OverlapRadius;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UBaseInteractableComponent* BaseInteractable;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UWidgetComponent* WidgetComponent;
	
public:	
	ABasePickupQuestItem();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;

protected:
	UPROPERTY(EditAnywhere, Category= "Defaults")
	UDataTable* InventoryDataTable;

	UPROPERTY(EditAnywhere, Category= "Defaults")
	FName InventoryID;

	UPROPERTY(EditAnywhere, Category= "Defaults")
	float InteractionDuration;

	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	UBaseQuestItem* ItemReference;

	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData ItemInteractionData;
	
	virtual void BeginPlay() override;

	void InitializeItem(const TSubclassOf<UBaseQuestItem>& BaseClass);
	void UpdateInteractionData();
	void ObtainedBy(const ABachelorThesisCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
