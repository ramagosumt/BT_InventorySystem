#include "StateTree/Tasks/AIAS_STT_SetMovementGait.h"

#include "Characters/AIAS_NPC.h"
#include "Controllers/AIAS_AIC.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FAIAS_STT_SetMovementGait::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AAIAS_NPC* Owner = Data.Owner;
	AAIAS_AIC* OwnerController = Data.OwnerController;
	if (!Owner || !OwnerController)
	{
		return EStateTreeRunStatus::Failed;
	}

	Owner->SetMovementGait(Data.MovementGait);

	return EStateTreeRunStatus::Running;
}	