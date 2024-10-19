#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UGraphDialogueComponent;
class UDialogueChoiceWidget;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SpeakerName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* OptionPanel;

	UPROPERTY()
	UGraphDialogueComponent* OwningDialogueComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueChoiceWidget> DesiredChoiceWidget;

public:
	void UpdateWidget(const int32 DialogueIndex, const FText& CharacterName, const FText& Dialogue, const TArray<FText>& Choices) const;
	
	FORCEINLINE void SetOwningDialogueComponent(UGraphDialogueComponent* NewDialogueComponent) { OwningDialogueComponent = NewDialogueComponent; }
	
};
