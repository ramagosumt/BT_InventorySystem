#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BasePickupItem.generated.h"

#define ECC_InventoryItem ECC_GameTraceChannel1

class UWidgetComponent;
class UBaseInteractableComponent;
class USphereComponent;
class UDataTable;
class UBaseItem;

UCLASS()
class BACHELORTHESIS_API ABasePickupItem : public AActor, public IInteractionInterface
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
	ABasePickupItem();

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
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category= "Defaults")
	float InteractionDuration;

	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	UBaseItem* ItemReference;

	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData ItemInteractionData;
	
	virtual void BeginPlay() override;

	void InitializeItem(const TSubclassOf<UBaseItem>& BaseClass, const int32 NewQuantity);
	void UpdateInteractionData();
	void ObtainedBy(const ABachelorThesisCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
