#include "Controllers/AIAS_AIC.h"

#include "Components/StateTreeAIComponent.h"

AAIAS_AIC::AAIAS_AIC()
{
	// ===== StateTree =====

	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	BrainComponent = StateTreeAIComponent;
}

void AAIAS_AIC::SetStateTree(UStateTree* StateTree)
{
	if (StateTreeAIComponent && StateTree)
	{
		StateTreeAIComponent->SetStateTree(StateTree);
	}
}