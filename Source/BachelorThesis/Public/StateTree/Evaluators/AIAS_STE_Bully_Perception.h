#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
#include "StateTreePropertyRef.h"

#include "AIAS_STE_Bully_Perception.generated.h"

class AAIAS_NPC_Bully;
class AAIAS_AIC_Bully;
class UAIPerceptionComponent;

/** Interprets Bully perception stimuli and exposes target awareness to the StateTree. */
UCLASS()
class BACHELORTHESIS_API UAIAS_STE_Bully_Perception : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
public:
	// ===== StateTree =====
	
	UPROPERTY(EditAnywhere, Category = "StateTree")
	TStateTreePropertyRef<FGameplayTag> Motivation;
	
	// ===== Output =====
	
	UPROPERTY(VisibleAnywhere, Category = "Output")
	TObjectPtr<AActor> AcquiredTarget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Output")
	FVector PredictedTargetLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Output")
	bool bIsStimulusReceived = false;

	UPROPERTY(VisibleAnywhere, Category = "Output")
	bool bIsSightStimulusReceived = false;

	UPROPERTY(VisibleAnywhere, Category = "Output")
	bool bIsTargetSeen = false;
	
protected:
	// ===== StateTree =====
	
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;
	virtual void Tick(FStateTreeExecutionContext& Context, float DeltaTime) override;
	
private:
	// ===== Context =====
	
	UPROPERTY()
	TObjectPtr<AAIAS_NPC_Bully> Owner = nullptr;

	UPROPERTY()
	TObjectPtr<AAIAS_AIC_Bully> OwnerController = nullptr;
	
	// ===== Focused Perception =====
	
	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> FocusedPerceptionComponent = nullptr;
	
	UFUNCTION()
	void OnFocusedTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnFocusedTargetPerceptionForgotten(AActor* Actor);
	
	void ReceiveFocusedSightStimulus(AActor* Actor);
	void ReceivePredictionStimulus(AActor* Actor, const FAIStimulus& Stimulus);
	
	// ===== Acquisition Perception =====
	
	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> PeripheralPerceptionComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> ProximityPerceptionComponent = nullptr;
	
	UFUNCTION()
	void OnTargetAcquisitionPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	// ===== Target =====

	void AcquireTarget(AActor* Actor);
	void LoseSightOfTarget();
	void ForgetTarget();
	
	// ===== Motivation =====
	
	FGameplayTag PendingMotivation;
	bool bHasPendingMotivation = false;

	void QueueMotivation(const FGameplayTag& NewMotivation);
};
