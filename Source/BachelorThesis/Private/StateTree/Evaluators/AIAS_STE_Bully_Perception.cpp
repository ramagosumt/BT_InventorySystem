#include "StateTree/Evaluators/AIAS_STE_Bully_Perception.h"

#include "Characters/AIAS_NPC_Bully.h"
#include "Controllers/AIAS_AIC_Bully.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Sight.h"

void UAIAS_STE_Bully_Perception::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);
	
	AcquiredTarget = nullptr;
	PredictedTargetLocation = FVector::ZeroVector;
	bIsStimulusReceived = false;
	bIsSightStimulusReceived = false;
	bIsTargetSeen = false;
	PendingMotivation = FGameplayTag();
	bHasPendingMotivation = false;
	
	Owner = Cast<AAIAS_NPC_Bully>(GetOwnerActor(Context));
	if (!Owner)
	{
		return;
	}

	OwnerController = Cast<AAIAS_AIC_Bully>(Owner->GetController());
	if (!OwnerController)
	{
		return;
	}

	FocusedPerceptionComponent = OwnerController->GetFocusedPerceptionComponent();
	if (!FocusedPerceptionComponent)
	{
		return;
	}
	
	PeripheralPerceptionComponent = OwnerController->GetPeripheralPerceptionComponent();
	if (!PeripheralPerceptionComponent)
	{
		return;
	}
	
	ProximityPerceptionComponent = OwnerController->GetProximityPerceptionComponent();
	if (!ProximityPerceptionComponent)
	{
		return;
	}
	
	FocusedPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionUpdated);
	FocusedPerceptionComponent->OnTargetPerceptionForgotten.AddUniqueDynamic(this, &UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionForgotten);
	PeripheralPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &UAIAS_STE_Bully_Perception::OnTargetAcquisitionPerceptionUpdated);
	ProximityPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &UAIAS_STE_Bully_Perception::OnTargetAcquisitionPerceptionUpdated);
}

void UAIAS_STE_Bully_Perception::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);
	
	if (ProximityPerceptionComponent)
	{
		ProximityPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &UAIAS_STE_Bully_Perception::OnTargetAcquisitionPerceptionUpdated);
	}
	
	if (PeripheralPerceptionComponent)
	{
		PeripheralPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &UAIAS_STE_Bully_Perception::OnTargetAcquisitionPerceptionUpdated);
	}
	
	if (FocusedPerceptionComponent)
	{
		FocusedPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionUpdated);
		FocusedPerceptionComponent->OnTargetPerceptionForgotten.RemoveDynamic(this, &UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionForgotten);
	}
	
	PeripheralPerceptionComponent = nullptr;
	FocusedPerceptionComponent = nullptr;
	OwnerController = nullptr;
	Owner = nullptr;
}

void UAIAS_STE_Bully_Perception::Tick(FStateTreeExecutionContext& Context, float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (!bHasPendingMotivation)
	{
		return;
	}

	FGameplayTag* MotivationPtr = Motivation.GetMutablePtr(Context);
	if (!MotivationPtr)
	{
		return;
	}

	*MotivationPtr = PendingMotivation;
	bHasPendingMotivation = false;
}

void UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		bIsStimulusReceived = true;

		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			ReceiveFocusedSightStimulus(Actor);
			return;
		}

		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Prediction>())
		{
			ReceivePredictionStimulus(Actor, Stimulus);
		}

		return;
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>() && Actor == AcquiredTarget.Get())
	{
		LoseSightOfTarget();
	}
}

void UAIAS_STE_Bully_Perception::OnFocusedTargetPerceptionForgotten(AActor* Actor)
{
	if (Actor != AcquiredTarget.Get())
	{
		return;
	}

	ForgetTarget();
}

void UAIAS_STE_Bully_Perception::ReceiveFocusedSightStimulus(AActor* Actor)
{
	bIsSightStimulusReceived = true;
	bIsTargetSeen = true;

	AcquireTarget(Actor);
}

void UAIAS_STE_Bully_Perception::ReceivePredictionStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
	if (Actor != AcquiredTarget.Get())
	{
		return;
	}

	PredictedTargetLocation = Stimulus.StimulusLocation;

	QueueMotivation(FGameplayTag::RequestGameplayTag(FName("Bully.Motivation.Pursue.TryFindTarget")));
}

void UAIAS_STE_Bully_Perception::OnTargetAcquisitionPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		OwnerController->SetFocus(AcquiredTarget.Get());
	}
}

void UAIAS_STE_Bully_Perception::AcquireTarget(AActor* Actor)
{
	if (!Actor || !OwnerController)
	{
		return;
	}

	AcquiredTarget = Actor;
	OwnerController->SetFocus(AcquiredTarget.Get());

	QueueMotivation(FGameplayTag::RequestGameplayTag(FName("Bully.Motivation.Pursue")));
}

void UAIAS_STE_Bully_Perception::LoseSightOfTarget()
{
	bIsTargetSeen = false;

	if (OwnerController)
	{
		OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	if (!Owner || !AcquiredTarget)
	{
		return;
	}

	UAISense_Prediction::RequestPawnPredictionEvent(Owner, AcquiredTarget.Get(), 1.f);
}

void UAIAS_STE_Bully_Perception::ForgetTarget()
{
	bIsStimulusReceived = false;
	bIsSightStimulusReceived = false;
	bIsTargetSeen = false;

	AcquiredTarget = nullptr;

	if (OwnerController)
	{
		OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	QueueMotivation(FGameplayTag::RequestGameplayTag(FName("Bully.Motivation.Patrol")));
}

void UAIAS_STE_Bully_Perception::QueueMotivation(const FGameplayTag& NewMotivation)
{
	PendingMotivation = NewMotivation;
	bHasPendingMotivation = true;
}