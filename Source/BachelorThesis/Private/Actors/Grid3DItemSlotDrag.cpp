#include "Actors/Grid3DItemSlotDrag.h"
#include "Objects/BaseItem.h"

AGrid3DItemSlotDrag::AGrid3DItemSlotDrag()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot =CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void AGrid3DItemSlotDrag::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemReference)
	{
		Mesh->SetStaticMesh(ItemReference->GetItemAssetData().Mesh);
	}
}

void AGrid3DItemSlotDrag::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ItemReference && Mesh && Mesh->GetStaticMesh())
	{
		if (ItemReference->GetItemRotated())
		{
			const FRotator InitialRotation = Mesh->GetComponentRotation();
			const FRotator DesiredRotation = FRotator(0.f, 90.f, 0.f);

			const FRotator CurrentRotation = FMath::RInterpTo(InitialRotation, DesiredRotation, DeltaSeconds, 5.f);
			Mesh->SetWorldRotation(CurrentRotation);
		}
		else
		{
			const FRotator InitialRotation = Mesh->GetComponentRotation();
			const FRotator DesiredRotation = FRotator(0.f, 0.f, 0.f);

			const FRotator CurrentRotation = FMath::RInterpTo(InitialRotation, DesiredRotation, DeltaSeconds, 5.f);
			Mesh->SetWorldRotation(CurrentRotation);
		}
	}
}

void AGrid3DItemSlotDrag::PositionItem(const float TileSize, const FVector2D TopLeftTile, const FVector& GridPosition)
{
	if (ItemReference && Mesh && Mesh->GetStaticMesh())
	{
		TopLeftPosition = TopLeftTile;
		
		const float ItemCenterX = (ItemReference->GetItemDimensions().X * 0.5f + TopLeftPosition.X) * TileSize;
		const float ItemCenterY = (ItemReference->GetItemDimensions().Y * 0.5f + TopLeftPosition.Y) * TileSize;

		constexpr float ItemZ = 50.f;

		const FVector CenterPosition = FVector(
			GridPosition.X + ItemCenterX,
			GridPosition.Y + ItemCenterY,
			GridPosition.Z + ItemZ
		);

		const float RightEdgeX = (TopLeftTile.X + ItemReference->GetItemDimensions().X) * TileSize;
        const float VerticalCenterY = (TopLeftTile.Y + ItemReference->GetItemDimensions().Y * 0.5f) * TileSize;
        
        CenterRightPosition = FVector(
            GridPosition.X + RightEdgeX,
            GridPosition.Y + VerticalCenterY,
            GridPosition.Z + ItemZ
        );

		Mesh->SetWorldLocation(CenterPosition);
	}
}

void AGrid3DItemSlotDrag::RotateItem() const
{
	if (ItemReference && Mesh && Mesh->GetStaticMesh())
	{
		if (ItemReference->GetItemRotated())
		{
			const FRotator DesiredRotation = FRotator(0.f, 90.f, 0.f);
			Mesh->SetWorldRotation(DesiredRotation);
		}
		else
		{
			const FRotator DesiredRotation = FRotator(0.f, 0.f, 0.f);
			Mesh->SetWorldRotation(DesiredRotation);
		}
	}
}

void AGrid3DItemSlotDrag::ScaleItem(const float TileSize) const
{
	if (ItemReference && Mesh && Mesh->GetStaticMesh())
	{
		FVector DesiredSize;
		if (ItemReference->GetItemRotated())
		{
			DesiredSize = FVector(ItemReference->GetItemDimensions().Y * TileSize, ItemReference->GetItemDimensions().X * TileSize, 1.f);
		}
		else
		{
			DesiredSize = FVector(ItemReference->GetItemDimensions().X * TileSize, ItemReference->GetItemDimensions().Y * TileSize, 1.f);
		}

		const FVector OriginalSize = Mesh->GetStaticMesh()->GetBounds().BoxExtent * 2.f;

		constexpr float Padding = 0.4f;
		const FVector DesiredScale = FVector((DesiredSize / OriginalSize).X - Padding, (DesiredSize / OriginalSize).Y - Padding, 1.f);

		Mesh->SetWorldScale3D(DesiredScale);
	}
}
