#include "Actors/Grid3DItemSlot.h"

#include "Actors/Grid3DInventory.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/BaseItem.h"
#include "UIs/ChoiceMenuWidget.h"

AGrid3DItemSlot::AGrid3DItemSlot()
{
	DefaultSceneRoot =CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	DesiredChoiceMenuClass = ConstructorHelpers::FClassFinder<UChoiceMenuWidget> (TEXT("/Game/Rama_InventorySystem/UIs/W_ChoiceMenu")).Class;
}

void AGrid3DItemSlot::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemReference)
	{
		Mesh->SetStaticMesh(ItemReference->GetItemAssetData().Mesh);
	}
}

void AGrid3DItemSlot::Destroyed()
{
	Super::Destroyed();

	if (ChoiceMenuWidget)
	{
		ChoiceMenuWidget->RemoveFromParent();
	}
}

void AGrid3DItemSlot::PositionItem(const float TileSize, const FVector2D TopLeftTile, const FVector& GridPosition)
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

void AGrid3DItemSlot::RotateItem() const
{
	if (ItemReference && Mesh && Mesh->GetStaticMesh())
	{
		if (ItemReference->GetItemRotated())
		{
			const FRotator Rotation = FRotator(0.f, 90.f, 0.f);
			Mesh->SetWorldRotation(Rotation);
		}
		else
		{
			const FRotator Rotation = FRotator(0.f, 0.f, 0.f);
			Mesh->SetWorldRotation(Rotation);
		}
	}
}

void AGrid3DItemSlot::ScaleItem(const float TileSize) const
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

void AGrid3DItemSlot::PopulateItemDimension(FVector2D& StartPoint, FVector2D& EndPoint) const
{
	if (ItemReference)
	{
		const float ItemXLength = ItemReference->GetItemDimensions().X;
		const float ItemYLength = ItemReference->GetItemDimensions().Y;

		const float GridXLength = TopLeftPosition.X + ItemXLength - 1;
		const float GridYLength = TopLeftPosition.Y + ItemYLength - 1;

		StartPoint = FVector2D(TopLeftPosition.X, TopLeftPosition.Y);
		EndPoint = FVector2D(GridXLength, GridYLength);
	}
}

void AGrid3DItemSlot::LMB()
{
	if (DesiredChoiceMenuClass)
	{
		if (ChoiceMenuWidget) ChoiceMenuWidget->RemoveFromParent();
		ChoiceMenuWidget = CreateWidget<UChoiceMenuWidget>(GetWorld(), DesiredChoiceMenuClass);
		ChoiceMenuWidget->SetOwning3DSlot(this);
		
		if (OwningInventory)
		{
			if (OwningInventory->GetCurrentChoiceMenu()) OwningInventory->ClearChoiceMenu();
			OwningInventory->SetCurrentChoiceMenu(ChoiceMenuWidget);
		}
		
		ChoiceMenuWidget->AddToViewport();
	}
}

FVector2D AGrid3DItemSlot::GetCenterRightPositionInViewport() const
{
	FVector2D CenterRightScreenPosition;
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, CenterRightPosition, CenterRightScreenPosition, true);
	}
	
	return CenterRightScreenPosition;
}
