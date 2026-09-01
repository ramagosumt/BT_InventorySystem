#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "StateTree/Tasks/AIAS_STT.h"

#include "AIAS_STT_MoveToRandomLocation.generated.h"

/** Instance data for generating a random reachable movement destination. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_MoveToRandomLocation_InstanceData : public FAIAS_STT_InstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm"))
	float MoveRadius = 500.f;
	
	UPROPERTY(EditAnywhere)
	TStateTreePropertyRef<FVector> MoveDestination;
};

/** Generates a random reachable navigation point around the owning AIAS NPC. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_MoveToRandomLocation : public FAIAS_STT
{
	GENERATED_BODY()
	
	using FInstanceDataType = FAIAS_STT_MoveToRandomLocation_InstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
