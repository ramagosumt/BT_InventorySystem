#include "StateTree/Conditions/AIAS_STC_HasSplinePath.h"

#include "Characters/AIAS_NPC.h"

FText UAIAS_STC_HasSplinePath::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return bHasSplinePath ? INVTEXT("Has Spline Path") : INVTEXT("Does Not Have Spline Path");
}

bool UAIAS_STC_HasSplinePath::TestCondition(FStateTreeExecutionContext& Context) const
{
	const AAIAS_NPC* Owner = Cast<AAIAS_NPC>(GetOwnerActor(Context));
	if (!Owner)
	{
		return false;
	}

	return Owner->HasSplinePath() == bHasSplinePath;
}
