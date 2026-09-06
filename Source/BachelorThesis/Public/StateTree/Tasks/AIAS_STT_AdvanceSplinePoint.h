#pragma once

#include "CoreMinimal.h"
#include "StateTree/Tasks/AIAS_STT.h"

#include "AIAS_STT_AdvanceSplinePoint.generated.h"

/** Advances the owning AIAS NPC to the next point along its spline path. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_AdvanceSplinePoint : public FAIAS_STT
{
	GENERATED_BODY()
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
