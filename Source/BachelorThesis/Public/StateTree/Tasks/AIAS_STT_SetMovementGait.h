#pragma once

#include "CoreMinimal.h"
#include "Characters/AIAS_NPC.h"
#include "StateTree/Tasks/AIAS_STT.h"

#include "AIAS_STT_SetMovementGait.generated.h"

/** Instance data for applying a movement gait to an AIAS NPC. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_SetMovementGait_InstanceData : public FAIAS_STT_InstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	EAIAS_NPC_MovementGait MovementGait = EAIAS_NPC_MovementGait::Walk;
};

/** Applies a configured movement gait for the lifetime of the active StateTree state. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_SetMovementGait : public FAIAS_STT
{
	GENERATED_BODY()
	
	FAIAS_STT_SetMovementGait()
	{
		bConsideredForCompletion = false;
	}
	
	using FInstanceDataType = FAIAS_STT_SetMovementGait_InstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
