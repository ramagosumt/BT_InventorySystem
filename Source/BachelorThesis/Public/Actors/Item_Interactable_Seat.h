#pragma once

#include "CoreMinimal.h"
#include "Actors/Item_Interactable_Root.h"
#include "Item_Interactable_Seat.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API AItem_Interactable_Seat : public AItem_Interactable_Root
{
	GENERATED_BODY()
	
public:
	AItem_Interactable_Seat();
	
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USceneComponent* SitLocation;
	
	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	USceneComponent* StandLocation;
	
	bool bIsBeingSatOn;
	
	void SitDown(ABachelorThesisCharacter* PlayerCharacter);
	void StandUp(ABachelorThesisCharacter* PlayerCharacter);

};
