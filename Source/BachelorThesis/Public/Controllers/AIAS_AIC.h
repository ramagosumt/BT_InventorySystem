#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIAS_AIC.generated.h"

class UStateTree;
class UStateTreeAIComponent;

/** Base AI controller for AIAS NPCs. */
UCLASS()
class BACHELORTHESIS_API AAIAS_AIC : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIAS_AIC();

protected:
	// ===== StateTree =====

	/**
	 * Assigns the StateTree asset executed by this controller.
	 *
	 * @param StateTree StateTree asset to assign.
	 */
	void SetStateTree(UStateTree* StateTree);

private:
	// ===== StateTree =====

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};
