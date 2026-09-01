#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIAS_STT.generated.h"

class AAIAS_NPC;
class AAIAS_AIC;

/** Shared instance data for AIAS StateTree tasks. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT_InstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Context")
	TObjectPtr<AAIAS_NPC> Owner = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Context")
	TObjectPtr<AAIAS_AIC> OwnerController = nullptr;
};

/** Base task for reusable AIAS StateTree behavior. */
USTRUCT()
struct BACHELORTHESIS_API FAIAS_STT : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FAIAS_STT_InstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
};
