#include "StateTree/Tasks/AIAS_STT_AdvanceSplinePoint.h"

#include "Characters/AIAS_NPC.h"
#include "Controllers/AIAS_AIC.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FAIAS_STT_AdvanceSplinePoint::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AAIAS_NPC* Owner = Data.Owner;
	AAIAS_AIC* OwnerController = Data.OwnerController;
	if (!Owner || !OwnerController)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Owner->AdvanceSplinePoint();

	return EStateTreeRunStatus::Succeeded;
}