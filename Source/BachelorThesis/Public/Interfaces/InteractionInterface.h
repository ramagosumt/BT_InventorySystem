#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ABachelorThesisCharacter;

UENUM()
enum class EInteractionMode : uint8
{
	InventoryItem UMETA(DisplayName= "InventoryItem"),
	NPC UMETA(DisplayName= "NPC"),
	Device UMETA(DisplayName= "Device"),
	Toggle UMETA(DisplayName= "Toggle"),
	Container UMETA(DisplayName= "Container")
};

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() :
	ItemInteractionMode(EInteractionMode::InventoryItem),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	Duration(0.0f)
	{
		
	};

	UPROPERTY(EditInstanceOnly)
	EInteractionMode ItemInteractionMode;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;
	
	UPROPERTY(EditInstanceOnly)
	uint8 Quantity;
	
	UPROPERTY(EditInstanceOnly)
	float Duration;
};

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BACHELORTHESIS_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter);

	FInteractionData InteractionData;
};
