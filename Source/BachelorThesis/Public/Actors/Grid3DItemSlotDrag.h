#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid3DItemSlotDrag.generated.h"

class UBaseItem;

UCLASS()
class BACHELORTHESIS_API AGrid3DItemSlotDrag : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UBaseItem* ItemReference;

	FVector2D TopLeftPosition;
	FVector CenterRightPosition;
	
public:	
	AGrid3DItemSlotDrag();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

	void PositionItem(const float TileSize, const FVector2D TopLeftTile, const FVector& GridPosition);
	void RotateItem() const;
	void ScaleItem(const float TileSize) const;

	FORCEINLINE UBaseItem* GetItemReference() const { return ItemReference; }

	FORCEINLINE void SetItemReference(UBaseItem* NewItemReference) { ItemReference = NewItemReference; }

};
