#include "Actors/II_Sittable.h"
#include "Characters/BachelorThesisCharacter.h"

AII_Sittable::AII_Sittable()
{
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
	if (PlayerCharacter) PlayerCharacter->PlayAnimation(FText::FromString("Sit"));
}

void AII_Sittable::StandUp(ABachelorThesisCharacter* PlayerCharacter)
{
	bIsBeingSatOn = false;
	if (PlayerCharacter) PlayerCharacter->PlayAnimation(FText::FromString("Stand"));
}
