#include "Actors/II_Sittable.h"
#include "Characters/BachelorThesisCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AII_Sittable::AII_Sittable()
{
	StandLocation = CreateDefaultSubobject<USceneComponent>(TEXT("StandLocation"));
	StandLocation->SetupAttachment(GetRootComponent());
	StandLocation->SetRelativeLocation(FVector(90.f, 0.0f, 0.0f));
	
	InteractionAction = FText::FromString(TEXT("sit down on"));
	InteractionName = FText::FromString(TEXT("chair"));
	InteractionDuration = 1.f;
}

void AII_Sittable::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	Super::Interact(PlayerCharacter);
		
	if (!bIsBeingSatOn) SitDown(PlayerCharacter);
		else StandUp(PlayerCharacter);
}

void AII_Sittable::SitDown(ABachelorThesisCharacter* PlayerCharacter)
{
	bIsBeingSatOn = true;
	
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsSitting(true);
		
		const FVector NewLocation(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		const FRotator NewRotation(0.f, GetActorRotation().Yaw, 0.f);
		
		// PlayerCharacter->GetCharacterMovement()->DisableMovement();
		// PlayerCharacter->bUseControllerRotationYaw = false;
		// PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		PlayerCharacter->SetActorLocation(NewLocation);
		PlayerCharacter->SetActorRotation(NewRotation);
	}
}

void AII_Sittable::StandUp(ABachelorThesisCharacter* PlayerCharacter)
{
	bIsBeingSatOn = false;
	
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsSitting(false);
		
		if (StandLocation)
		{
			const FVector NewLocation(StandLocation->GetComponentLocation().X, StandLocation->GetComponentLocation().Y, StandLocation->GetComponentLocation().Z + PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
			const FRotator NewRotation(0.f, GetActorRotation().Yaw, 0.f);
			
			PlayerCharacter->SetActorLocation(NewLocation);
			PlayerCharacter->SetActorRotation(NewRotation);
		}
	}
}
