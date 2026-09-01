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
	
	// ===== Perception =====
	
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, true);
	SightConfig->PointOfViewBackwardOffset = 250.f;
	SightConfig->NearClippingRadius = 175.f;
	SightConfig->SetMaxAge(5.f);
	
	PredictionConfig = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("PredictionConfig"));
	PredictionConfig->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*PredictionConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}
