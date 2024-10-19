#include "Characters/BaseNPC.h"
#include "Components/BaseInteractableComponent.h"
#include "Components/GraphDialogueComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "UIs/InteractionButtonWidget.h"

ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh> (TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny")).Object;
	if (GetMesh() && SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f,-90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f,0.f));
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_NPC);
	}

	BaseInteractable = CreateDefaultSubobject<UBaseInteractableComponent>(TEXT("BaseInteractable"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetClass(ConstructorHelpers::FClassFinder<UInteractionButtonWidget> (TEXT("/Game/Rama_InteractionSystem/UIs/W_InteractionButton")).Class);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetHiddenInGame(true);

	NameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameComponent"));
	NameComponent->SetupAttachment(GetRootComponent());
	NameComponent->SetText(FText::FromName(CharacterName));
	NameComponent->SetHorizontalAlignment(EHTA_Center);
	NameComponent->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	NameComponent->SetTextRenderColor(FColor::Red);
	NameComponent->SetVisibility(false);

	UpdateInteractionData();

	DialogueComponent = CreateDefaultSubobject<UGraphDialogueComponent>(TEXT("DialogueComponent"));
}

void ABaseNPC::OnConstruction(const FTransform& Transform)
{
	NameComponent->SetText(FText::FromName(CharacterName));
}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateInteractionData();
}

void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseNPC::BeginFocus()
{
	if (NameComponent) NameComponent->SetVisibility(true);
}

void ABaseNPC::EndFocus()
{
	if (NameComponent) NameComponent->SetVisibility(false);
}

void ABaseNPC::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	if (DialogueComponent)
	{
		DialogueComponent->OpenConversation();
	}
}

void ABaseNPC::UpdateInteractionData()
{
	NPCInteractionData.ItemInteractionMode = EInteractionMode::NPC;
	NPCInteractionData.Action = FText::FromName("talk to");
	NPCInteractionData.Name = FText::FromName(CharacterName);
	NPCInteractionData.Duration = InteractionDuration;
	
	InteractionData = NPCInteractionData;

	if (BaseInteractable) BaseInteractable->SetInteractionDuration(NPCInteractionData.Duration);
	if (DialogueComponent) DialogueComponent->SetCharacterName(CharacterName);
}
