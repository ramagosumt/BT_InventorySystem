#include "Actors/TimeManager.h"
#include "Components/CentralTimeComponent.h"
#include "Frameworks/MainGameState.h"
#include "Components/LightComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/DirectionalLight.h"

ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SunsetTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SunsetTimeline"));
}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
	GameState->GetCentralTimeComponent()->OnMorning.AddDynamic(this, &ATimeManager::ExecuteSunrise);
	GameState->GetCentralTimeComponent()->OnEvening.AddDynamic(this, &ATimeManager::ExecuteSunset);
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeManager::ExecuteSunrise()
{
	if (DirectionalLight)
	{
		if (SunMovement)
		{
			SunsetTimeline->SetPlayRate(GameState->GetCentralTimeComponent()->GetTimeScale());
			
			FOnTimelineFloat UpdatedFloat;
			FOnTimelineEvent FinishedEvent;
			
			UpdatedFloat.BindUFunction(this, FName("SunriseStart"));
			
			SunsetTimeline->AddInterpFloat(SunMovement, UpdatedFloat);
			SunsetTimeline->SetTimelineFinishedFunc(FinishedEvent);
			SunsetTimeline->SetLooping(false);
			SunsetTimeline->ReverseFromEnd();
		}
	}
}

void ATimeManager::ExecuteSunset()
{
	if (DirectionalLight)
	{
		if (SunMovement)
		{
			SunsetTimeline->SetPlayRate(GameState->GetCentralTimeComponent()->GetTimeScale());
			
			FOnTimelineFloat UpdatedFloat;
			FOnTimelineEvent FinishedEvent;
			
			UpdatedFloat.BindUFunction(this, FName("SunsetStart"));
			
			SunsetTimeline->AddInterpFloat(SunMovement, UpdatedFloat);
			SunsetTimeline->SetTimelineFinishedFunc(FinishedEvent);
			SunsetTimeline->SetLooping(false);
			SunsetTimeline->PlayFromStart();
		}
	}
}

void ATimeManager::SunriseStart(float Value)
{
	DirectionalLight->GetLightComponent()->SetIntensity(FMath::Clamp(Value * 3.f, 0.25f, 3.f));
	DirectionalLight->GetLightComponent()->SetTemperature(FMath::Clamp(Value * 6500.f, 4500.f, 6500.f));
}


void ATimeManager::SunsetStart(float Value)
{
	DirectionalLight->GetLightComponent()->SetIntensity(FMath::Clamp(Value * 3.f, 0.25f, 3.f));
	DirectionalLight->GetLightComponent()->SetTemperature(FMath::Clamp(Value * 6500.f, 4500.f, 6500.f));
}
