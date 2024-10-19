#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

struct FInteractionData;
class UPlayerMenuWidget;
class UInteractionBarWidget;
class ABachelorThesisCharacter;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API AMainHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	ABachelorThesisCharacter* MainCharacter;

	TSubclassOf<UPlayerMenuWidget> DesiredPlayerMenu;
	TSubclassOf<UInteractionBarWidget> DesiredInteractionBar;

	UPROPERTY()
	UPlayerMenuWidget* PlayerMenu;

	UPROPERTY()
	UInteractionBarWidget* InteractionBar;

	int32 DesiredGridTileSize;
	bool bIsPlayerMenuOn;

	bool bIsCinematicModeOn;
	
public:
	AMainHUD();
	
	void ToggleInventoryMenu();
	void UpdateInteractionBar(const FInteractionData& NewInteractionData) const;
	void ShowInteractionBar() const;
	void HideInteractionBar() const;

	FORCEINLINE int32 GetTileSize() const { return DesiredGridTileSize; }
	FORCEINLINE bool IsPlayerMenuOn() const { return bIsPlayerMenuOn; }
	FORCEINLINE void SetCinematicMode(const bool NewCinematicMode) { bIsCinematicModeOn = NewCinematicMode; }

protected:
	virtual void BeginPlay() override;
	
};
