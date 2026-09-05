#include "Controllers/AIAS_AIC_Bully.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISenseConfig_Sight.h"
#include "StateTree.h"

AAIAS_AIC_Bully::AAIAS_AIC_Bully()
{
	// ===== StateTree =====
	static ConstructorHelpers::FObjectFinder<UStateTree> BullyStateTree(TEXT("/Game/Rama_AIAS/StateTrees/ST_Bully_Root"));
	if (BullyStateTree.Succeeded())
	{
		SetStateTree(BullyStateTree.Object);
	}
	
	// ===== Focused Perception =====
	
	FocusedAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("FocusedAIPerceptionComponent"));
	SetPerceptionComponent(*FocusedAIPerceptionComponent);
	
	FocusedSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("FocusedSightConfig"));
	FocusedSightConfig->SightRadius = 2000.f;
	FocusedSightConfig->LoseSightRadius = 2500.f;
	FocusedSightConfig->PeripheralVisionAngleDegrees = 30.f;
	FocusedSightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, true);
	FocusedSightConfig->SetMaxAge(5.f);
	
	PredictionConfig = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("PredictionConfig"));
	PredictionConfig->SetMaxAge(5.f);

	FocusedAIPerceptionComponent->ConfigureSense(*FocusedSightConfig);
	FocusedAIPerceptionComponent->ConfigureSense(*PredictionConfig);
	FocusedAIPerceptionComponent->SetDominantSense(FocusedSightConfig->GetSenseImplementation());
	
	// ===== Peripheral Perception =====
	
	PeripheralAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PeripheralAIPerceptionComponent"));
	
	PeripheralSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("PeripheralSightConfig"));
	PeripheralSightConfig->SightRadius = 1000.f;
	PeripheralSightConfig->LoseSightRadius = 1000.f;
	PeripheralSightConfig->PeripheralVisionAngleDegrees = 100.f;
	PeripheralSightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, true);
	PeripheralSightConfig->SetMaxAge(1.f);

	PeripheralAIPerceptionComponent->ConfigureSense(*PeripheralSightConfig);
	PeripheralAIPerceptionComponent->SetDominantSense(PeripheralSightConfig->GetSenseImplementation());
	
	// ===== Proximity Perception =====
	
	ProximityAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("ProximityAIPerceptionComponent"));
	
	ProximitySightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("ProximitySightConfig"));
	ProximitySightConfig->SightRadius = 150.f;
	ProximitySightConfig->LoseSightRadius = 150.f;
	ProximitySightConfig->PeripheralVisionAngleDegrees = 360.f;
	ProximitySightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, true);
	ProximitySightConfig->SetMaxAge(1.f);

	ProximityAIPerceptionComponent->ConfigureSense(*ProximitySightConfig);
	ProximityAIPerceptionComponent->SetDominantSense(ProximitySightConfig->GetSenseImplementation());
}

UAIPerceptionComponent* AAIAS_AIC_Bully::GetFocusedPerceptionComponent() const
{
	return FocusedAIPerceptionComponent;
}

UAIPerceptionComponent* AAIAS_AIC_Bully::GetPeripheralPerceptionComponent() const
{
	return PeripheralAIPerceptionComponent;
}

UAIPerceptionComponent* AAIAS_AIC_Bully::GetProximityPerceptionComponent() const
{
	return ProximityAIPerceptionComponent;
}