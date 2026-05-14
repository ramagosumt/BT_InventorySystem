#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseDialogueData.generated.h"

USTRUCT()
struct FDialogueChoiceData
{
	GENERATED_BODY()

	FDialogueChoiceData() {}

	UPROPERTY(EditAnywhere)
	FName ChoiceDialogue;
	
	UPROPERTY(EditAnywhere)
	FName NextRow;
};

USTRUCT()
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

	FDialogueData() :
	bIsDialogueEnd(false), bHasChoice(false)
	{};
	
	UPROPERTY(EditAnywhere, Category= "DialogueData")
	FText CurrentDialogue;
	
	UPROPERTY(EditAnywhere, Category= "DialogueData")
	FName NextRow;
	
	UPROPERTY(EditAnywhere, Category= "DialogueData")
	bool bIsDialogueEnd;
	
	UPROPERTY(EditAnywhere, Category= "DialogueData")
	bool bHasChoice;
	
	UPROPERTY(EditAnywhere, Category= "DialogueData")
	TArray<FDialogueChoiceData> ChoiceData;
};
