#include "UIs/GridSlotDragVisualWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/BaseItem.h"

void UGridSlotDragVisualWidget::RefreshWidget()
{
	if (ItemReference)
	{
		BackgroundBorder->SetBrushColor(UKismetMathLibrary::MakeColor(.5f, .5f, .5f, .2f));
		
		const float WidgetSizeX = ItemReference->GetItemDimensions().X * TileSize;
		const float WidgetSizeY = ItemReference->GetItemDimensions().Y * TileSize;

		Size = FVector2D(WidgetSizeX, WidgetSizeY);

		BackgroundSizeBox->SetWidthOverride(Size.X);
		BackgroundSizeBox->SetHeightOverride(Size.Y);
		
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ItemImage->Slot)) CanvasPanelSlot->SetSize(Size);
		
		if (ItemImage)
		{
			if (!IsValid(ItemImageMaterial))
			{
				if (DesiredItemImageMaterial)
				{
					ItemImageMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DesiredItemImageMaterial);
				}
			}
			
			if (ItemImageMaterial)
			{
				ItemImageMaterial->SetTextureParameterValue(TEXT("ItemImage"), ItemReference->GetItemAssetData().Icon);
				if (ItemReference->GetItemRotated()) ItemImageMaterial->SetScalarParameterValue(TEXT("RotationAngle"), .75f);
					else ItemImageMaterial->SetScalarParameterValue(TEXT("RotationAngle"), 0.f);

				ItemImage->SetBrushFromMaterial(ItemImageMaterial);
			}
			else
			{
				if (ItemReference->GetItemRotated()) ItemImage->SetRenderTransformAngle(90.f);
					else ItemImage->SetRenderTransformAngle(0.f);
						
				ItemImage->SetBrushFromTexture(ItemReference->GetItemAssetData().Icon);
			}
		}

		if (ItemReference->GetItemNumericData().bIsStackable)
		{
			if (ItemQuantity) ItemQuantity->SetText(FText::AsNumber(ItemReference->GetItemQuantity()));
		}
		else
		{
			if (ItemQuantity) ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
