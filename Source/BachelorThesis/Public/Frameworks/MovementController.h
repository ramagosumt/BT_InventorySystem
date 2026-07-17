#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MovementController.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API AMovementController : public APlayerController
{
	GENERATED_BODY()
	
	AMovementController();
	
	virtual void OnPossess(APawn* InPawn) override;
};
