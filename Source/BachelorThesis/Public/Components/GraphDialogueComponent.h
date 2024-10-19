#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GraphDialogueComponent.generated.h"

class UDialogueWidget;

UENUM()
enum class EDialogueState : uint8
{
	PassThrough UMETA(DisplayName = "PassThrough"),
	Updated UMETA(DisplayName = "Updated") 
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UGraphDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

	FName OwnerName;

public:
	UGraphDialogueComponent();

	void OpenConversation();
	void CloseConversation() const;

	FORCEINLINE void UpdatePath(const int32 Path, const int32 Choice) { DialoguePath.Add(Path, Choice); ProgressDialogue(); }
	FORCEINLINE void SetCharacterName(const FName NewCharacterName) { OwnerName = NewCharacterName; }

protected:
	TSubclassOf<UDialogueWidget> DesiredDialogueWidget;
	
	UPROPERTY()
	UDialogueWidget* DialogueWidget;

	float DialogueIndex;
	TMap<int32, int32> DialoguePath;
	int32 DialogueTreeBranch;
	
	virtual void ExecuteDialogue();
	EDialogueState AddDialogue(const FText& CharacterName, const FText& Dialogue, const TArray<FText>& Choices, int32& ChoiceIndex);

	FORCEINLINE void ProgressDialogue() { DialogueIndex = -1; ExecuteDialogue(); }
	FORCEINLINE void ClearPath() { DialoguePath.Empty(); }
	FORCEINLINE void RemovePathFrom(const int32 Threshold) { for (int i = 0; i < Threshold; i++) DialoguePath.Remove(DialogueIndex - i); }
	FORCEINLINE void SwitchTreeBranch(const int32 NewTreeIndex) { DialogueTreeBranch = NewTreeIndex; ClearPath(); }
	
};
