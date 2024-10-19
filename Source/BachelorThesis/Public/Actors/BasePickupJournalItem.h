#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BasePickupJournalItem.generated.h"

class UGDC_JournalItem;
class UBoxComponent;
class UBaseInteractableComponent;
class USphereComponent;
class UWidgetComponent;
class UDataTable;
class UBaseJournalItem;

#define ECC_InventoryItem ECC_GameTraceChannel1

UCLASS()
class BACHELORTHESIS_API ABasePickupJournalItem : public AActor, public IInteractionInterface
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

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UBoxComponent* InteractionArea;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UGDC_JournalItem* DialogueComponent;

	UPROPERTY()
	ABachelorThesisCharacter* CurrentInteractingActor;
	
public:	
	ABasePickupJournalItem();

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
	UBaseJournalItem* ItemReference;

	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData ItemInteractionData;
	
	virtual void BeginPlay() override;

	void InitializeItem(const TSubclassOf<UBaseJournalItem>& BaseClass);
	void UpdateInteractionData();
	void ObtainedBy(const ABachelorThesisCharacter* Taker);

	UFUNCTION()
	void OnDialogueEnd();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
