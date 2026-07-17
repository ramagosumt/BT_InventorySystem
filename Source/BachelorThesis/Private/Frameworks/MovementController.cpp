#include "Frameworks/MovementController.h"

AMovementController::AMovementController()
{
	bAutoManageActiveCameraTarget = false;
}

void AMovementController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetViewTargetWithBlend(InPawn);
}
