#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueChoiceWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UGraphDialogueComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ChoiceDialogue;

	UPROPERTY(meta=(BindWidget))
	UButton* ChoiceButton;
	
	UPROPERTY()
	UGraphDialogueComponent* OwningDialogueComponent;

	FText ChoiceText;
	int32 ChoiceIndex;
	int32 DialogueIndex;

public:
	FORCEINLINE void SetOwningDialogueComponent(UGraphDialogueComponent* NewDialogueComponent) { OwningDialogueComponent = NewDialogueComponent; }
	FORCEINLINE void SetChoiceText(const FText& NewChoiceText) { ChoiceText = NewChoiceText; }
	FORCEINLINE void SetChoiceIndex(const int32 NewChoiceIndex) { ChoiceIndex = NewChoiceIndex; }
	FORCEINLINE void SetDialogueIndex(const int32 NewDialogueIndex) { DialogueIndex = NewDialogueIndex; }
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChoiceHovered();

	UFUNCTION()
	void OnChoiceUnhovered();

	UFUNCTION()
	void UpdateDialogueComponent();
	
};
