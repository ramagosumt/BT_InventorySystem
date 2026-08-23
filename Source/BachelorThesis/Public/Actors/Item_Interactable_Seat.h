#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseInteractableItem.h"
#include "II_Sittable.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API AII_Sittable : public ABaseInteractableItem
{
	GENERATED_BODY()
	
public:
	AII_Sittable();
	
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USceneComponent* StandLocation;
	
	bool bIsBeingSatOn;
	
	void SitDown(ABachelorThesisCharacter* PlayerCharacter);
	void StandUp(ABachelorThesisCharacter* PlayerCharacter);

};
