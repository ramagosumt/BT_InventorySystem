#include "Actors/Grid3DInventory.h"
#include "KismetProceduralMeshLibrary.h"
#include "Actors/Grid3DItemSlot.h"
#include "Actors/Grid3DItemSlotDrag.h"
#include "Camera/CameraComponent.h"
#include "Components/GridInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AGrid3DInventory::AGrid3DInventory()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot =CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->SetupAttachment(GetRootComponent());

	InventoryCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InventoryCamera"));
	InventoryCamera->SetupAttachment(GetRootComponent());
	InventoryCamera->bUsePawnControlRotation = false;
	InventoryCamera->SetRelativeLocation(FVector(-50.f, 0.f, 150.f));
	InventoryCamera->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));

	DesiredInventoryMaterial = ConstructorHelpers::FObjectFinder<UMaterialInstance> (TEXT("/Game/Rama_InventorySystem/3D/M_Test3DGrid_Inst")).Object;
}

void AGrid3DInventory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ClearMeshData();
	
	GenerateInventoryMesh(GetSizeX(), GetSizeY(), GetTileSize());

	PositionCamera();
	SwitchView();
}

void AGrid3DInventory::Destroyed()
{
	if (CurrentSlotDrag)
	{
		if (GridInventory) GridInventory->HandleAddItem(CurrentSlotDrag->GetItemReference());

		CurrentSlotDrag->Destroy();
		
		ActorInFocused = nullptr;
		CurrentSlotDrag = nullptr;
		ComponentInFocused = nullptr;
		DraggedItem = nullptr;
		DistanceToActorInFocused = 0.f;
	}

	DestroyChildren();
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		const FInputModeGameOnly GameOnly;
		PlayerController->SetInputMode(GameOnly);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetCinematicMode(false, true, true);
	}
}

void AGrid3DInventory::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FVector WorldPosition;
		FVector WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		constexpr float TraceDistance = 1000.f;

		const FVector TraceStart = WorldPosition;
		const FVector TraceEnd = TraceStart + WorldDirection * TraceDistance;

		FHitResult TraceHit;

		if (bIsGrabbing && ComponentInFocused)
		{
			const FVector GridPlaneNormalVector = FVector(0.0f, 0.0f, 1.0f);
			const float GridZPosition = ProceduralMesh->GetComponentLocation().Z;
		
			float DotProduct = FVector::DotProduct(GridPlaneNormalVector, WorldDirection);
			if (FMath::Abs(DotProduct) > KINDA_SMALL_NUMBER)
			{
				const float Distance = (GridZPosition - FVector::DotProduct(GridPlaneNormalVector, WorldPosition)) / DotProduct;
				const FVector IntersectPoint = WorldPosition + WorldDirection * Distance;
				
				const FVector DesiredGrabHeight = FVector(0.f, 0.f, 125.f);
				const FVector InitialGrabLocation = ComponentInFocused->GetComponentLocation();
				const FVector DesiredGrabLocation = IntersectPoint + DesiredGrabHeight;
				
				const FVector CurrentGrabLocation = FMath::VInterpTo(InitialGrabLocation, DesiredGrabLocation, DeltaSeconds, 5.f);
				ComponentInFocused->SetWorldLocation(CurrentGrabLocation);

				FindMousePositionInTile();

				FVector2D MouseStart;
				FVector2D MouseEnd;
				PopulateDragItemDimension(MouseStart, MouseEnd);

				if (GridInventory)
				{
					const int32 DraggedItemTopLeftIndex = GridInventory->TileToIndex(FTile(CurrentHoveredTile.X, CurrentHoveredTile.Y));
					if (GridInventory->IsSpaceAvailable(DraggedItem, DraggedItemTopLeftIndex))
					{
						StartHighlightingBox(MouseStart, MouseEnd, FLinearColor::Green);
					}
					else
					{
						StartHighlightingBox(MouseStart, MouseEnd, FLinearColor::Red);
					}
				}
				
				return;
			}
		}

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility))
		{
			if (TraceHit.GetActor() != ActorInFocused)
			{
				ActorInFocused = TraceHit.GetActor();
				FoundInteraction();

				if (TraceHit.GetComponent()->Mobility == EComponentMobility::Type::Movable)
				{
					ComponentInFocused = TraceHit.GetComponent();
					DistanceToActorInFocused = TraceHit.Distance;
				}
				
				return;
			}

			if (TraceHit.GetActor() == ActorInFocused) return;
		}

		FoundNoInteraction();
	}
}

void AGrid3DInventory::GenerateInventoryMesh(const int32 Rows, const int32 Columns, const float Size)
{
	if (ProceduralMesh)
	{
		ProceduralMesh->ClearAllMeshSections();

		for (int32 i = 0; i <= Rows; i++)
		{
			for (int32 j = 0; j <= Columns; j++)
			{
				const FVector CurrentVertices = FVector(i * GetTileSize(), j * GetTileSize(), 0.f);
				Vertices.Add(CurrentVertices);

				const FVector2D CurrentUV = FVector2D(static_cast<float>(i) / Rows, static_cast<float>(j) / Columns);
				UVs.Add(CurrentUV);

				UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Rows + 1, Columns + 1, false, Triangles);
				UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
			}
		}

		if (DesiredInventoryMaterial)
		{
			InventoryMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DesiredInventoryMaterial);

			if (InventoryMaterial)
			{
				InventoryMaterial->SetScalarParameterValue(FName(TEXT("Rows")), static_cast<float>(1) / static_cast<float>(GetSizeX()));
				InventoryMaterial->SetScalarParameterValue(FName(TEXT("Columns")), static_cast<float>(1) / static_cast<float>(GetSizeY()));
				InventoryMaterial->SetVectorParameterValue(FName(TEXT("LineColor")), FLinearColor::White);
				InventoryMaterial->SetVectorParameterValue(FName(TEXT("TileColor")), FLinearColor::Black);
				InventoryMaterial->SetVectorParameterValue(FName(TEXT("StartPoint")), UKismetMathLibrary::MakeColor(ItemBox.StartPoint.X, ItemBox.StartPoint.Y, 0.f, 0.f));
				InventoryMaterial->SetVectorParameterValue(FName(TEXT("EndPoint")), UKismetMathLibrary::MakeColor(ItemBox.EndPoint.X, ItemBox.EndPoint.Y, 0.f, 0.f));
				InventoryMaterial->SetVectorParameterValue(FName(TEXT("HighlightColor")), FLinearColor::Green);
				InventoryMaterial->SetScalarParameterValue(FName(TEXT("IsChecking")), bIsChecking);
			
				ProceduralMesh->SetMaterial(0, InventoryMaterial);
			}
		}
		
		ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, false);
	}
}

void AGrid3DInventory::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
	Tangents.Empty();
}

void AGrid3DInventory::PositionCamera() const
{
	if (InventoryCamera)
	{
		const float InventoryWidth = GetSizeX() * GetTileSize();
		const float InventoryHeight = GetSizeY() * GetTileSize();

		const FVector InventoryCenter = FVector(InventoryWidth / 2.0f, InventoryHeight / 2.0f, 0.0f);

		constexpr float CameraHeight = 800.0f;
		const FVector CameraPosition = InventoryCenter + FVector(0.0f, 100.f, CameraHeight);
		InventoryCamera->SetRelativeLocation(CameraPosition);

		FRotator CameraRotation = (InventoryCenter - CameraPosition).Rotation();
		CameraRotation.Pitch = -80.f;
		CameraRotation.Yaw = -90.f;
		InventoryCamera->SetRelativeRotation(CameraRotation);
	}
}

void AGrid3DInventory::SwitchView()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameAndUI GameAndUI;
		GameAndUI.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(GameAndUI);
		PlayerController->SetViewTargetWithBlend(this);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetCinematicMode(true, true, true);
	}
}

void AGrid3DInventory::InitializeInventory(UGridInventoryComponent* NewInventory, const float NewTileSize)
{
	SetSizeX(NewInventory->GetSizeX());
	SetSizeY(NewInventory->GetSizeY());
	SetTileSize(NewTileSize);
	SetOwningInventory(NewInventory);
	
	if (GridInventory) GridInventory->OnInventoryUpdated.AddUObject(this, &AGrid3DInventory::RefreshInventory);

	RefreshInventory();
}

void AGrid3DInventory::LMB()
{
	if (ActorInFocused)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FVector WorldDirection;
			PlayerController->DeprojectMousePositionToWorld(MouseClickPosition, WorldDirection);
		}
		
		if (AGrid3DItemSlot* FocusedItem = Cast<AGrid3DItemSlot>(ActorInFocused))
		{
			FocusedItem->LMB();
		}
	}
}

void AGrid3DInventory::IsLMBing()
{
	if (ActorInFocused)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FVector WorldPosition;
			FVector WorldDirection;
			PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

			if (MouseClickPosition != WorldPosition)
			{
				bIsGrabbing = true;
				StopHighlightingBox();

				if (!CurrentSlotDrag)
				{
					FTransform NewTransform;
					NewTransform.SetLocation(FVector(0.f));
					
					CurrentSlotDrag = GetWorld()->SpawnActorDeferred<AGrid3DItemSlotDrag>(AGrid3DItemSlotDrag::StaticClass(), NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (CurrentSlotDrag)
					{
						if (const AGrid3DItemSlot* ItemSlot = Cast<AGrid3DItemSlot>(ActorInFocused))
						{
							DraggedItem = ItemSlot->GetItemReference();
							const FTile* TopLeftTile = GridInventory->GetAllItems().Find(DraggedItem);
							const FVector2D TopLeft = FVector2D(TopLeftTile->X, TopLeftTile->Y);
						
							CurrentSlotDrag->SetItemReference(Cast<AGrid3DItemSlot>(ActorInFocused)->GetItemReference());
							if (GridInventory) GridInventory->RemoveSingleInstanceOfItem(DraggedItem);
							CurrentSlotDrag->FinishSpawning(NewTransform);
							CurrentSlotDrag->PositionItem(GetTileSize(), TopLeft, ProceduralMesh->GetComponentLocation());
							CurrentSlotDrag->RotateItem();
							CurrentSlotDrag->ScaleItem(GetTileSize());

							ActorInFocused = CurrentSlotDrag;
							ComponentInFocused = CurrentSlotDrag->FindComponentByClass<UPrimitiveComponent>();
						}
					}
				}
			}
		}
	}
}

void AGrid3DInventory::StopLMB()
{
	if (ActorInFocused)
	{
		if (bIsGrabbing)
		{
			const int32 DraggedItemTopLeftIndex = GridInventory->TileToIndex(FTile(CurrentHoveredTile.X, CurrentHoveredTile.Y));

			if (CurrentHoveredTile == FVector2D(-999, -999))
			{
				CurrentSlotDrag->Destroy();
				StopHighlightingBox();
		
				ActorInFocused = nullptr;
				CurrentSlotDrag = nullptr;
				ComponentInFocused = nullptr;
				DraggedItem = nullptr;
				DistanceToActorInFocused = 0.f;

				bIsGrabbing = false;
				
				return;
			}
			
			if (GridInventory->IsSpaceAvailable(DraggedItem, DraggedItemTopLeftIndex))
			{
				GridInventory->AddNewItem(DraggedItem, DraggedItemTopLeftIndex, DraggedItem->GetItemQuantity());
				
				CurrentSlotDrag->Destroy();
				StopHighlightingBox();
		
				ActorInFocused = nullptr;
				CurrentSlotDrag = nullptr;
				ComponentInFocused = nullptr;
				DraggedItem = nullptr;
				DistanceToActorInFocused = 0.f;

				bIsGrabbing = false;
				
				return;
			}

			FItemAddResult AddResult = GridInventory->HandleAddItem(DraggedItem);
			if (AddResult == FItemAddResult::AddedAll(DraggedItem->GetItemQuantity())) UE_LOG(LogTemp, Warning, TEXT("DUH"))

			CurrentSlotDrag->Destroy();
			StopHighlightingBox();
		
			ActorInFocused = nullptr;
			CurrentSlotDrag = nullptr;
			ComponentInFocused = nullptr;
			DraggedItem = nullptr;
			DistanceToActorInFocused = 0.f;

			bIsGrabbing = false;
		}
	}
}

void AGrid3DInventory::RotateItem()
{
	if (DraggedItem)
	{
		DraggedItem->SetItemRotated();
	}
}

void AGrid3DInventory::RefreshInventory()
{
	if (GridInventory)
	{
		DestroyChildren();

		TArray<UBaseItem*> GridInventoryKeys;
		GridInventory->GetAllItems().GetKeys(GridInventoryKeys);

		for (UBaseItem* Item : GridInventoryKeys)
		{
			FTransform NewTransform;
			NewTransform.SetLocation(FVector(0.f));
			
			if (AGrid3DItemSlot* ItemSlot = GetWorld()->SpawnActorDeferred<AGrid3DItemSlot>(AGrid3DItemSlot::StaticClass(), NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				const FTile* TopLeftTile = GridInventory->GetAllItems().Find(Item);
				const FVector2D TopLeft = FVector2D(TopLeftTile->X, TopLeftTile->Y);
				
				ItemSlots.Add(ItemSlot);
				ItemSlot->SetOwningInventory(this);
				ItemSlot->SetInventoryReference(GridInventory);
				ItemSlot->SetItemReference(Item);
				ItemSlot->FinishSpawning(NewTransform);
				ItemSlot->PositionItem(GetTileSize(), TopLeft, ProceduralMesh->GetComponentLocation());
				ItemSlot->RotateItem();
				ItemSlot->ScaleItem(GetTileSize());
			}
		}
	}
}

void AGrid3DInventory::DestroyChildren()
{
	for (AGrid3DItemSlot* ItemSlot : ItemSlots)
	{
		ItemSlot->Destroy();
	}

	ItemSlots.Empty();
}

void AGrid3DInventory::FoundInteraction()
{
	if (const AGrid3DItemSlot* FocusedItem = Cast<AGrid3DItemSlot>(ActorInFocused))
	{
		FocusedItem->PopulateItemDimension(ItemBox.StartPoint, ItemBox.EndPoint);
		StartHighlightingBox(ItemBox.StartPoint, ItemBox.EndPoint, FLinearColor::Yellow);
	}
}

void AGrid3DInventory::FoundNoInteraction()
{
	if (ActorInFocused)
	{
		StopHighlightingBox();

		ActorInFocused = nullptr;
		ComponentInFocused = nullptr;

		DistanceToActorInFocused = 0.f;
	}
}

void AGrid3DInventory::StartHighlightingBox(const FVector2D& StartPoint, const FVector2D& EndPoint, const FLinearColor& HighlightColor)
{
	bIsChecking = true;

	if (InventoryMaterial)
	{
		InventoryMaterial->SetVectorParameterValue(FName(TEXT("StartPoint")), UKismetMathLibrary::MakeColor(StartPoint.X, StartPoint.Y, 0.f, 0.f));
		InventoryMaterial->SetVectorParameterValue(FName(TEXT("EndPoint")), UKismetMathLibrary::MakeColor(EndPoint.X, EndPoint.Y, 0.f, 0.f));
		InventoryMaterial->SetVectorParameterValue(FName(TEXT("HighlightColor")), HighlightColor);
		InventoryMaterial->SetScalarParameterValue(FName(TEXT("IsChecking")), bIsChecking);
	}
}

void AGrid3DInventory::StopHighlightingBox()
{
	bIsChecking = false;

	if (InventoryMaterial)
	{
		InventoryMaterial->SetVectorParameterValue(FName(TEXT("StartPoint")), UKismetMathLibrary::MakeColor(ItemBox.StartPoint.X, ItemBox.StartPoint.Y, 0.f, 0.f));
		InventoryMaterial->SetVectorParameterValue(FName(TEXT("EndPoint")), UKismetMathLibrary::MakeColor(ItemBox.EndPoint.X, ItemBox.EndPoint.Y, 0.f, 0.f));
		InventoryMaterial->SetScalarParameterValue(FName(TEXT("IsChecking")), bIsChecking);
	}
}

void AGrid3DInventory::PopulateDragItemDimension(FVector2D& StartPoint, FVector2D& EndPoint) const
{
	if (DraggedItem)
	{
		const float ItemXLength = DraggedItem->GetItemDimensions().X;
		const float ItemYLength = DraggedItem->GetItemDimensions().Y;

		const float GridXLength = CurrentHoveredTile.X + ItemXLength - 1;
		const float GridYLength = CurrentHoveredTile.Y + ItemYLength - 1;

		StartPoint = FVector2D(CurrentHoveredTile.X, CurrentHoveredTile.Y);
		EndPoint = FVector2D(GridXLength, GridYLength);
	}
}

void AGrid3DInventory::FindMousePositionInTile()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FVector WorldPosition;
		FVector WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		const FVector GridPlaneNormalVector = FVector(0.0f, 0.0f, 1.0f);
		const float GridZPosition = ProceduralMesh->GetComponentLocation().Z;

		float DotProduct = FVector::DotProduct(GridPlaneNormalVector, WorldDirection);
		if (FMath::Abs(DotProduct) > KINDA_SMALL_NUMBER)
		{
			const float Distance = (GridZPosition - FVector::DotProduct(GridPlaneNormalVector, WorldPosition)) / DotProduct;
			const FVector IntersectPoint = WorldPosition + WorldDirection * Distance;
				
			const int32 TileX = FMath::FloorToInt((IntersectPoint.X - ProceduralMesh->GetComponentLocation().X) / TileSize);
			const int32 TileY = FMath::FloorToInt((IntersectPoint.Y - ProceduralMesh->GetComponentLocation().Y) / TileSize);
				
			if (TileX >= 0 && TileX < GetSizeX() && TileY >= 0 && TileY < GetSizeY())
			{
				CurrentHoveredTile = FVector2D(TileX, TileY);
			}
			else
			{
				CurrentHoveredTile = FVector2D(-999, -999); 
			}
		}
		else
		{
			CurrentHoveredTile = FVector2D(-999, -999);
		}
	}
}