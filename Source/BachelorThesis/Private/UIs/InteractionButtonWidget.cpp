#include "UIs/InteractionButtonWidget.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

void UInteractionButtonWidget::SetInteractionPercent(const float Percentage)
{
	if (!IsValid(RoundProgressBarMaterial))
	{
		if (DesiredRoundProgressBar)
		{
			RoundProgressBarMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DesiredRoundProgressBar);
			if (RoundProgressBar) RoundProgressBar->SetBrushFromMaterial(RoundProgressBarMaterial);
		}
	}

	RoundProgressBarMaterial->SetScalarParameterValue(TEXT("Percent"), Percentage);
}
