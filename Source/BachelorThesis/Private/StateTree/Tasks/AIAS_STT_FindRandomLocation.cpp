#include "StateTree/Tasks/AIAS_STT_FindRandomLocation.h"

#include "Characters/AIAS_NPC.h"
#include "Controllers/AIAS_AIC.h"
#include "NavigationSystem.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FAIAS_STT_FindRandomLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AAIAS_NPC* Owner = Data.Owner;
	AAIAS_AIC* OwnerController = Data.OwnerController;
	if (!Owner || !OwnerController)
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector* MoveDestination = Data.MoveDestination.GetMutablePtr(Context);
	if (!MoveDestination)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	const UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavigationSystem)
	{
		return EStateTreeRunStatus::Failed;
	}

	FNavLocation ResultLocation;
	if (!NavigationSystem->GetRandomPointInNavigableRadius(Owner->GetActorLocation(), Data.MoveRadius, ResultLocation))
	{
		return EStateTreeRunStatus::Failed;
	}

	*MoveDestination = ResultLocation.Location;

	return EStateTreeRunStatus::Succeeded;
}	