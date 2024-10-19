#include "BachelorThesis/Public/Characters/BachelorThesisCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actors/Grid3DInventory.h"
#include "Components/GridInventoryComponent.h"
#include "Components/ListInventoryComponent.h"
#include "Components/SlotInventoryComponent.h"
#include "Components/TraceInteractionComponent.h"
#include "Frameworks/MainHUD.h"
#include "Objects/BaseItem.h"
#include "Objects/BaseJournalItem.h"
#include "Objects/BaseQuestItem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ABachelorThesisCharacter::ABachelorThesisCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetMesh(), "Head");
	FPCamera->SetRelativeLocation(FVector(0.f, -10.f,10.f));
	FPCamera->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	FPCamera->bUsePawnControlRotation = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GridInventory = CreateDefaultSubobject<UGridInventoryComponent>(TEXT("GridInventory"));
	GridInventory->SetSizeX(10);
	GridInventory->SetSizeY(7);

	SlotInventory = CreateDefaultSubobject<USlotInventoryComponent>(TEXT("SlotInventory"));
	SlotInventory->SetSize(6);

	ListInventory = CreateDefaultSubobject<UListInventoryComponent>(TEXT("ListInventory"));

	InteractionCheckFrequency = 0.03f;
	InteractionCheckDistance = 200.f;

	TraceInteraction = CreateDefaultSubobject<UTraceInteractionComponent>(TEXT("TraceInteraction"));
	TraceInteraction->SetComponentTickInterval(InteractionCheckFrequency);
	TraceInteraction->SetTraceDistance(InteractionCheckDistance);

	DesiredInventoryData = ConstructorHelpers::FObjectFinder<UDataTable> (TEXT("/Game/Rama_InventorySystem/Data/DT_BaseItem")).Object;
}

void ABachelorThesisCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	InitializeInventories();
}

void ABachelorThesisCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABachelorThesisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABachelorThesisCharacter::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABachelorThesisCharacter::Look);

		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ABachelorThesisCharacter::ToggleInventory);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABachelorThesisCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ABachelorThesisCharacter::EndInteract);

		EnhancedInputComponent->BindAction(SwitchInventoryDimensionAction, ETriggerEvent::Completed, this, &ABachelorThesisCharacter::SwitchInventoryDimension);

		EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Started, this, &ABachelorThesisCharacter::LMB);
		EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Triggered, this, &ABachelorThesisCharacter::IsLMBing);
		EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Completed, this, &ABachelorThesisCharacter::StopLMB);

		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &ABachelorThesisCharacter::RotateItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABachelorThesisCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABachelorThesisCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABachelorThesisCharacter::ToggleInventory()
{
	if (!bIs3D)
	{
		if (MainHUD) MainHUD->ToggleInventoryMenu();
	}
	else
	{
		if (InventoryBag)
		{
			InventoryBag->Destroy();
			InventoryBag = nullptr;
		}
		else
		{
			SpawnInventoryBag();
		}
	}
}

void ABachelorThesisCharacter::SwitchInventoryDimension()
{
	if (!bIs3D)
	{
		if (MainHUD)
		{
			if (MainHUD->IsPlayerMenuOn())
			{
				MainHUD->ToggleInventoryMenu();
				SpawnInventoryBag();
			}
		}
	}
	else
	{
		if (InventoryBag)
		{
			InventoryBag->Destroy();
			InventoryBag = nullptr;

			if (MainHUD) MainHUD->ToggleInventoryMenu();
		}
	}
	
	bIs3D = !bIs3D;
}

void ABachelorThesisCharacter::LMB()
{
	if (InventoryBag)
	{
		InventoryBag->LMB();
	}
}

void ABachelorThesisCharacter::IsLMBing()
{
	if (InventoryBag)
	{
		InventoryBag->IsLMBing();
	}
}

void ABachelorThesisCharacter::StopLMB()
{
	if (InventoryBag)
	{
		InventoryBag->StopLMB();
	}
}

void ABachelorThesisCharacter::RotateItem()
{
	if (InventoryBag)
	{
		InventoryBag->RotateItem();
	}
}

void ABachelorThesisCharacter::BeginInteract()
{
	if (!bIsInteracting)
	{
		if (TraceInteraction) TraceInteraction->HandleInteraction(true);
		bIsInteracting = true;
	}
}

void ABachelorThesisCharacter::EndInteract()
{
	if (bIsInteracting)
	{
		if (TraceInteraction) TraceInteraction->HandleInteraction(false);
		bIsInteracting = false;
	}
}

void ABachelorThesisCharacter::ResetInteract()
{
	if (TraceInteraction) TraceInteraction->ResetInteraction();
}

void ABachelorThesisCharacter::InitializeInventories()
{
	InitializeQuestInventory();
	InitializeCommonInventory();
	InitializeJournalInventory();
}

void ABachelorThesisCharacter::InitializeQuestInventory()
{
	if (DesiredInventoryData)
	{
		for (const auto ID : QuestItemIDs)
		{
			if (const FItemData* ItemData = DesiredInventoryData->FindRow<FItemData>(ID, ID.ToString()))
			{
				UBaseQuestItem* QuestItem = NewObject<UBaseQuestItem>(this, UBaseQuestItem::StaticClass());
			
				QuestItem->SetItemID(ItemData->ID);
				QuestItem->SetItemType(ItemData->Type);
				QuestItem->SetItemTextData(ItemData->TextData);
				QuestItem->SetItemAssetData(ItemData->AssetData);
			
				if (SlotInventory) SlotInventory->AddItem(QuestItem);
			}
		}
	}
}

void ABachelorThesisCharacter::InitializeCommonInventory()
{
	if (DesiredInventoryData)
	{
		for (const auto ID : CommonItemIDs)
		{
			if (const FItemData* ItemData = DesiredInventoryData->FindRow<FItemData>(ID, ID.ToString()))
			{
				UBaseItem* Item = NewObject<UBaseItem>(this, UBaseItem::StaticClass());
			
				Item->SetItemID(ItemData->ID);
				Item->SetItemType(ItemData->Type);
				Item->SetItemTextData(ItemData->TextData);
				Item->SetItemNumericData(ItemData->NumericData);
				Item->SetItemAssetData(ItemData->AssetData);
			
				if (GridInventory) GridInventory->HandleAddItem(Item);
			}
		}
	}
}

void ABachelorThesisCharacter::InitializeJournalInventory()
{
	if (DesiredInventoryData)
	{
		for (const auto ID : CommonItemIDs)
		{
			if (const FItemData* ItemData = DesiredInventoryData->FindRow<FItemData>(ID, ID.ToString()))
			{
				UBaseJournalItem* Item = NewObject<UBaseJournalItem>(this, UBaseJournalItem::StaticClass());
			
				Item->SetItemID(ItemData->ID);
				Item->SetItemType(ItemData->Type);
				Item->SetItemTextData(ItemData->TextData);
				Item->SetItemAssetData(ItemData->AssetData);
			
				if (ListInventory) ListInventory->AddItem(Item);
			}
		}
	}
}

void ABachelorThesisCharacter::SpawnInventoryBag()
{
	constexpr float Height = 50000.f;
	const FVector InventoryLocation = FVector(500000.f, 500000.f, Height);
			
	FTransform NewTransform;
	NewTransform.SetLocation(InventoryLocation);
		
	InventoryBag = GetWorld()->SpawnActorDeferred<AGrid3DInventory>(AGrid3DInventory::StaticClass(), NewTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (InventoryBag)
	{
		if (GridInventory) if (MainHUD)
		{
			InventoryBag->InitializeInventory(GridInventory, MainHUD->GetTileSize());
		}
				
		InventoryBag->FinishSpawning(NewTransform);
	}
}
