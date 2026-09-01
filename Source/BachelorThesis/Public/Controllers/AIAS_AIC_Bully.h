#pragma once

#include "CoreMinimal.h"
#include "Controllers/AIAS_AIC.h"

#include "AIAS_AIC_Bully.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Prediction;
class UAISenseConfig_Sight;

/** AIAS controller specialization for bully behavior and perception. */
UCLASS()
class BACHELORTHESIS_API AAIAS_AIC_Bully : public AAIAS_AIC
{
	GENERATED_BODY()
	
public:
	AAIAS_AIC_Bully();
	
private:
	
	// ===== Perception =====
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Prediction> PredictionConfig;
};
