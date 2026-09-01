#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "GameplayTagContainer.h"

#include "AIAS_STC_CompareGameplayTags.generated.h"

/** Compares two Gameplay Tags using hierarchical or exact matching. */
UCLASS()
class BACHELORTHESIS_API UAIAS_STC_CompareGameplayTags : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTag LeftTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTag RightTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	bool bExactMatch = false;
	
private:
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const override;
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
};
 