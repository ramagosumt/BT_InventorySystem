#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "StateTree/Tasks/AIAS_STT.h"

#include "AIAS_STT_FindCurrentSplineLocation.generated.h"

/** Instance data for resolving the next spline-path movement destination. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_FindCurrentSplineLocation_InstanceData : public FAIAS_STT_InstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Pathing")
	TStateTreePropertyRef<FVector> MoveDestination;
};

/** Resolves the next movement destination from the owning AIAS NPC's spline path. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_FindCurrentSplineLocation : public FAIAS_STT
{
	GENERATED_BODY()
	
	using FInstanceDataType = FAIAS_STT_FindCurrentSplineLocation_InstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
