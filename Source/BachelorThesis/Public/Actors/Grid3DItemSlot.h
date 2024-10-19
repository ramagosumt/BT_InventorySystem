#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid3DItemSlot.generated.h"

class UChoiceMenuWidget;
class UGridInventoryComponent;
class UBaseItem;
class AGrid3DInventory;

UCLASS()
class BACHELORTHESIS_API AGrid3DItemSlot : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UChoiceMenuWidget* ChoiceMenuWidget;

	UPROPERTY()
	TSubclassOf<UChoiceMenuWidget> DesiredChoiceMenuClass;

	UPROPERTY()
	AGrid3DInventory* OwningInventory;

	UPROPERTY()
	UGridInventoryComponent* InventoryReference;

	UPROPERTY()
	UBaseItem* ItemReference;

	FVector2D TopLeftPosition;
	FVector CenterRightPosition;
	
public:	
	AGrid3DItemSlot();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;

	void PositionItem(const float TileSize, const FVector2D TopLeftTile, const FVector& GridPosition);
	void RotateItem() const;
	void ScaleItem(const float TileSize) const;
	void PopulateItemDimension(FVector2D& StartPoint, FVector2D& EndPoint) const;
	void LMB();
	FVector2D GetCenterRightPositionInViewport() const;

	FORCEINLINE UGridInventoryComponent* GetInventoryReference() const { return InventoryReference; }
	FORCEINLINE UBaseItem* GetItemReference() const { return ItemReference; }

	FORCEINLINE void SetOwningInventory(AGrid3DInventory* NewOwningInventory) { OwningInventory = NewOwningInventory; }
	FORCEINLINE void SetInventoryReference(UGridInventoryComponent* NewInventoryReference) { InventoryReference = NewInventoryReference; }
	FORCEINLINE void SetItemReference(UBaseItem* NewItemReference) { ItemReference = NewItemReference; }
	
};
