#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BaseDialogueData.h"
#include "Interfaces/InteractionInterface.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnd);

class UDataTable;
class UDialogueWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UDialogueComponent : public UActorComponent, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	FOnDialogueEnd OnDialogueEnd;
	
	UDialogueComponent();
	
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;
	
	FORCEINLINE void SetCharacterName(const FName NewCharacterName) { CharacterName = NewCharacterName; };
	
	void SelectChoiceDialogue(const int32 ChoiceIndex);
		
private:
	UPROPERTY()
	FName CharacterName;
	
	UPROPERTY(EditAnywhere, Category= "Defaults")
	UDataTable* DialogueDataTable;
	
	UPROPERTY()
	FName DialogueRow;
	
	UPROPERTY()
	bool bIsDialogueEnd;
	
	TSubclassOf<UDialogueWidget> DesiredDialogueWidget;
	
	UPROPERTY()
	UDialogueWidget* DialogueWidget;
	
	void HandleUI(const FDialogueData* DialogueData);
	void HandleCinematic(const bool bCinematicMode) const;
	void UpdateDialogueWidget(const FDialogueData* DialogueData);
};
