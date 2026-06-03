#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CentralTimeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMorning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEvening);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMonth, int32, Months);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewDay, int32, Days);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewHour, int32, Hours);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMinute, int32, Minutes);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UCentralTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnMorning OnMorning;
	FOnEvening OnEvening;
	FOnNewMonth OnNewMonth;
	FOnNewDay OnNewDay;
	FOnNewHour OnNewHour;
	FOnNewMinute OnNewMinute;
	
	UCentralTimeComponent();
	
	virtual void BeginPlay() override;
	
	void PauseTime();
	void ResumeTime();
	void UpdateTimeScale(float NewScale);
	
	FORCEINLINE float GetTimeScale() const { return CentralTimeScale; }

private:	
	UPROPERTY( EditAnywhere )
	float CentralDayTimeInSeconds;
	
	UPROPERTY( EditAnywhere )
	float CentralTimeScale;
	
	int32 Months;
	int32 Days;
	int32 Hours;
	int32 Minutes;
	
	FTimerHandle CentralTimerHandle;
	
	void AdvanceTime();
	void ExecuteCentralTimer();
		
};
