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
	
	// ===== Perception =====
	
	UAIPerceptionComponent* GetFocusedPerceptionComponent() const;
	UAIPerceptionComponent* GetPeripheralPerceptionComponent() const;
	UAIPerceptionComponent* GetProximityPerceptionComponent() const;
	
private:
	// ===== Focused Perception =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> FocusedAIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception|Focused", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> FocusedSightConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception|Focused", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Prediction> PredictionConfig;
	
	// ===== Peripheral Perception =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PeripheralAIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception|Peripheral", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> PeripheralSightConfig;
	
	// ===== Proximity Perception =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> ProximityAIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIAS|Perception|Proximity", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> ProximitySightConfig;
};
