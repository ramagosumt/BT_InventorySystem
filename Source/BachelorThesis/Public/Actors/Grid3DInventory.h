#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "UIs/ChoiceMenuWidget.h"
#include "GameFramework/Actor.h"
#include "Grid3DInventory.generated.h"

class UProceduralMeshComponent;
class UCameraComponent;
class UGridInventoryComponent;
class UChoiceMenuWidget;
class UBaseItem;
class AGrid3DItemSlot;
class AGrid3DItemSlotDrag;

USTRUCT()
struct FTilesBox
{
	GENERATED_BODY()

	FVector2D StartPoint;
	FVector2D EndPoint;
};

UCLASS()
class BACHELORTHESIS_API AGrid3DInventory : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* InventoryCamera;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* DesiredInventoryMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* InventoryMaterial;

	UPROPERTY()
	UGridInventoryComponent* GridInventory; 

	FVector2D InventorySize;
	float TileSize;

	FTilesBox ItemBox;
	bool bIsChecking;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	TArray<AGrid3DItemSlot*> ItemSlots;

	UPROPERTY()
	AActor* ActorInFocused;

	UPROPERTY()
	UPrimitiveComponent* ComponentInFocused;

	UPROPERTY()
	AGrid3DItemSlotDrag* CurrentSlotDrag;

	UPROPERTY()
	UBaseItem* DraggedItem;

	float DistanceToActorInFocused;
	bool bIsGrabbing;
	FVector MouseClickPosition;

	UPROPERTY(VisibleInstanceOnly)
	FVector2D CurrentHoveredTile;

	UPROPERTY()
	UChoiceMenuWidget* CurrentChoiceMenu;

	void GenerateInventoryMesh(const int32 Rows, const int32 Columns, const float Size);
	void ClearMeshData();
	void PositionCamera() const;
	void SwitchView();
	void RefreshInventory();
	void DestroyChildren();
	void FoundInteraction();
	void FoundNoInteraction();
	void StartHighlightingBox(const FVector2D& StartPoint, const FVector2D& EndPoint, const FLinearColor& HighlightColor);
	void StopHighlightingBox();
	void PopulateDragItemDimension(FVector2D& StartPoint, FVector2D& EndPoint) const;
	void FindMousePositionInTile();

public:
	AGrid3DInventory();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaSeconds) override;

	void InitializeInventory(UGridInventoryComponent* NewInventory, const float NewTileSize);
	void LMB();
	void IsLMBing();
	void StopLMB();
	void RotateItem();

	FORCEINLINE int32 GetSizeX() const { return InventorySize.X; }
	FORCEINLINE int32 GetSizeY() const { return InventorySize.Y; }
	FORCEINLINE float GetTileSize() const { return TileSize; }
	FORCEINLINE UGridInventoryComponent* GetOwningInventory() const { return GridInventory; }
	FORCEINLINE UChoiceMenuWidget* GetCurrentChoiceMenu() const { return CurrentChoiceMenu; }

	FORCEINLINE void SetSizeX(const int32 NewX) { InventorySize.X = NewX; }
	FORCEINLINE void SetSizeY(const int32 NewY) { InventorySize.Y = NewY; }
	FORCEINLINE void SetTileSize(const float NewSize) { TileSize = NewSize; }
	FORCEINLINE void SetOwningInventory(UGridInventoryComponent* NewInventory) { GridInventory = NewInventory; }
	FORCEINLINE void SetCurrentChoiceMenu(UChoiceMenuWidget* NewChoiceMenu) { CurrentChoiceMenu = NewChoiceMenu; }

	FORCEINLINE void ClearChoiceMenu() { CurrentChoiceMenu->RemoveFromParent(); CurrentChoiceMenu = nullptr; }
	
};
