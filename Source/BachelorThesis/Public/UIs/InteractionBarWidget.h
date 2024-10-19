#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionBarWidget.generated.h"

class UTextBlock;
struct FInteractionData;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UInteractionBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* InteractionText;
	
public:
	void UpdateWidget(const FInteractionData& InteractionData) const;
	
};
