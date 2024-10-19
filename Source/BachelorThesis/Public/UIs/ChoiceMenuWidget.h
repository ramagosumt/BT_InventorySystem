#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceMenuWidget.generated.h"

class UVerticalBox;
class UButton;
class UGridInventorySlotWidget;
class AGrid3DItemSlot;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API UChoiceMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ChoiceMenu;

	UPROPERTY(meta=(BindWidget))
	UButton* UseButton;

	UPROPERTY(meta=(BindWidget))
	UButton* DropButton;

	UPROPERTY(meta=(BindWidget))
	UButton* DiscardButton;

	UPROPERTY()
	UGridInventorySlotWidget* OwningSlot;
	
	UPROPERTY()
	AGrid3DItemSlot* Owning3DSlot;

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void DropItem();

	UFUNCTION()
	void DiscardItemSlot();

public:
	FORCEINLINE void SetOwningSlot(UGridInventorySlotWidget* NewOwningSlot) { OwningSlot = NewOwningSlot; }
	FORCEINLINE void SetOwning3DSlot(AGrid3DItemSlot* NewOwning3DSlot) { Owning3DSlot = NewOwning3DSlot; }

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
