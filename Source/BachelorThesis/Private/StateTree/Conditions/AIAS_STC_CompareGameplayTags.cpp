#include "StateTree/Conditions/AIAS_STC_CompareGameplayTags.h"

FText UAIAS_STC_CompareGameplayTags::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	if (bExactMatch)
	{
		return FText::Format(INVTEXT("Gameplay Tag Exactly Matches: ({0})"), FText::FromName(RightTag.GetTagName()));
	}

	return FText::Format(INVTEXT("Gameplay Tag Matches: ({0})"), FText::FromName(RightTag.GetTagName()));
}

bool UAIAS_STC_CompareGameplayTags::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (bExactMatch)
	{
		return LeftTag.MatchesTagExact(RightTag);
	}

	return LeftTag.MatchesTag(RightTag);
}
