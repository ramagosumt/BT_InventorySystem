#pragma once

#include "CoreMinimal.h"
#include "Components/GraphDialogueComponent.h"
#include "GDC_JournalItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnd);

class ABachelorThesisCharacter;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UGDC_JournalItem : public UGraphDialogueComponent
{
	GENERATED_BODY()

public:
	FOnDialogueEnd OnDialogueEnd;

protected:
	virtual void ExecuteDialogue() override;
	
};
