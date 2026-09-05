#include "Characters/AIAS_NPC.h"

#include "Components/BaseInteractableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DialogueComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Controllers/AIAS_AIC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UIs/InteractionButtonWidget.h"

AAIAS_NPC::AAIAS_NPC()
{
	PrimaryActorTick.bCanEverTick = false;
		
	// ===== Character =====
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny"));
	if (DefaultMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(DefaultMesh.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> DefaultAnimInstance(TEXT("/Game/Rama_AIAS/Animations/ABP_Manny_NPC"));
	if (DefaultAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(DefaultAnimInstance.Class);
	}
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_NPC);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	AIControllerClass = AAIAS_AIC::StaticClass();
	
	// ===== Identity =====
	
	NameTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameTextComponent"));
	NameTextComponent->SetupAttachment(GetRootComponent());
	NameTextComponent->SetHorizontalAlignment(EHTA_Center);
	NameTextComponent->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	NameTextComponent->SetTextRenderColor(FColor::Red);
	NameTextComponent->SetVisibility(false);
	
	// ===== Movement =====
	
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->bRequestedMoveUseAcceleration = true;
		MovementComponent->GetNavMovementProperties()->bUseAccelerationForPaths = true;
	}

	// ===== Interaction =====
	
	InteractionComponent = CreateDefaultSubobject<UBaseInteractableComponent>(TEXT("InteractionComponent"));
	
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetHiddenInGame(true);
	
	static ConstructorHelpers::FClassFinder<UInteractionButtonWidget> InteractionWidgetClass(TEXT("/Game/Rama_InteractionSystem/UIs/W_InteractionButton"));
	if (InteractionWidgetClass.Succeeded())
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass.Class);
	}
	
	// ===== Dialogue =====
	
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
	
	UpdateInteractionData();
}

void AAIAS_NPC::SetMovementGait(EAIAS_NPC_MovementGait NewGait)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	switch (NewGait)
	{
	case EAIAS_NPC_MovementGait::Sneak:
		MovementComponent->MaxWalkSpeed = MovementProfile.SneakSpeed;
		break;

	case EAIAS_NPC_MovementGait::Walk:
		MovementComponent->MaxWalkSpeed = MovementProfile.WalkSpeed;
		break;

	case EAIAS_NPC_MovementGait::Run:
		MovementComponent->MaxWalkSpeed = MovementProfile.RunSpeed;
		break;

	case EAIAS_NPC_MovementGait::Sprint:
		MovementComponent->MaxWalkSpeed = MovementProfile.SprintSpeed;
		break;
	}
}

void AAIAS_NPC::ShowInteractionWidget() const
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetHiddenInGame(false);
	}
}

void AAIAS_NPC::HideInteractionWidget() const
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetHiddenInGame(true);
	}
}

void AAIAS_NPC::BeginFocus()
{
	IInteractionInterface::BeginFocus();

	if (NameTextComponent)
	{
		NameTextComponent->SetVisibility(true);
	}
}

void AAIAS_NPC::EndFocus()
{
	IInteractionInterface::EndFocus();

	if (NameTextComponent)
	{
		NameTextComponent->SetVisibility(false);
	}
}

void AAIAS_NPC::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);

	if (DialogueComponent)
	{
		DialogueComponent->Interact(PlayerCharacter);
	}
}

void AAIAS_NPC::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateInteractionData();
}

void AAIAS_NPC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (NameTextComponent)
	{
		NameTextComponent->SetText(FText::FromName(CharacterName));
	}

	UpdateInteractionData();
}

void AAIAS_NPC::UpdateInteractionData()
{
	NPCInteractionData.ItemInteractionMode = EInteractionMode::NPC;
	NPCInteractionData.Action = NSLOCTEXT("AIAS", "TalkToAction", "Talk to");
	NPCInteractionData.Name = FText::FromName(CharacterName);
	NPCInteractionData.Duration = InteractionDuration;

	InteractionData = NPCInteractionData;

	if (InteractionComponent)
	{
		InteractionComponent->SetInteractionDuration(InteractionDuration);
	}

	if (DialogueComponent)
	{
		DialogueComponent->SetCharacterName(CharacterName);
	}
}