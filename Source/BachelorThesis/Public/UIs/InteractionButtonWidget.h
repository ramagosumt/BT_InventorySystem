#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionButtonWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UInteractionButtonWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* DesiredRoundProgressBar;

	UPROPERTY()
	UMaterialInstanceDynamic* RoundProgressBarMaterial;

	UPROPERTY(meta=(BindWidget))
	UImage* RoundProgressBar;

public:
	void SetInteractionPercent(const float Percentage);
	
};
