#include "Actors/Item_Interactable_Seat.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AItem_Interactable_Seat::AItem_Interactable_Seat()
{
	SitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SitLocation"));
	SitLocation->SetupAttachment(GetRootComponent());
	SitLocation->SetRelativeLocation(FVector(40.f, 0.0f, 14.f));
	
	StandLocation = CreateDefaultSubobject<USceneComponent>(TEXT("StandLocation"));
	StandLocation->SetupAttachment(GetRootComponent());
	StandLocation->SetRelativeLocation(FVector(90.f, 0.0f, 0.0f));
	
	InteractionAction = FText::FromString(TEXT("sit down on"));
	InteractionName = FText::FromString(TEXT("chair"));
	InteractionDuration = 1.f;
}

void AItem_Interactable_Seat::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	Super::Interact(PlayerCharacter);
		
	if (!bIsBeingSatOn) SitDown(PlayerCharacter);
		else StandUp(PlayerCharacter);
}

void AItem_Interactable_Seat::SitDown(ABachelorThesisCharacter* PlayerCharacter)
{
	bIsBeingSatOn = true;
	
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsSitting(true);
		
		const FVector NewLocation(SitLocation->GetComponentLocation());
		const FRotator NewRotation(0.f, GetActorRotation().Yaw, 0.f);
		
		// PlayerCharacter->GetCharacterMovement()->DisableMovement();
		// PlayerCharacter->bUseControllerRotationYaw = false;
		// PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		PlayerCharacter->SetActorLocation(NewLocation);
		PlayerCharacter->SetActorRotation(NewRotation);
	}
}

void AItem_Interactable_Seat::StandUp(ABachelorThesisCharacter* PlayerCharacter)
{
	bIsBeingSatOn = false;
	
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsSitting(false);
		
		const FVector NewLocation(StandLocation->GetComponentLocation().X, StandLocation->GetComponentLocation().Y, StandLocation->GetComponentLocation().Z + PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		const FRotator NewRotation(0.f, GetActorRotation().Yaw, 0.f);
			
		PlayerCharacter->SetActorLocation(NewLocation);
		PlayerCharacter->SetActorRotation(NewRotation);
	}
}
