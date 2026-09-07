#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"

#include "AIAS_STC_HasSplinePath.generated.h"

/** Tests whether the owning AIAS NPC has an assigned spline path. */
UCLASS()
class BACHELORTHESIS_API UAIAS_STC_HasSplinePath : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Pathing")
	bool bHasSplinePath = true;
	
private:
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const override;
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
};
 