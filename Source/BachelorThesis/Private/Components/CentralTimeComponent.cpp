#include "Components/CentralTimeComponent.h"

UCentralTimeComponent::UCentralTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	Hours = 8;
	Minutes = 30;
	
	CentralDayTimeInSeconds = 30.f;
	CentralTimeScale = 1.f;
}

void UCentralTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ExecuteCentralTimer();
}

void UCentralTimeComponent::PauseTime()
{
	GetWorld()->GetTimerManager().ClearTimer(CentralTimerHandle);
	CentralTimerHandle.Invalidate();
}

void UCentralTimeComponent::ResumeTime()
{
	ExecuteCentralTimer();
}

void UCentralTimeComponent::UpdateTimeScale(float NewScale)
{
	PauseTime();
	CentralTimeScale = NewScale;
	ResumeTime();
}

void UCentralTimeComponent::AdvanceTime()
{
	{ Minutes++; OnNewMinute.Broadcast(Minutes); }
	if (Minutes >= 60)
	{
		{ Minutes = 0; Hours++; OnNewHour.Broadcast(Hours); if (Hours == 5) OnMorning.Broadcast(); if (Hours == 17) OnEvening.Broadcast(); } 
		if (Hours >= 24)
		{
			{ Hours = 0; Days++; OnNewDay.Broadcast(Days); }
			switch (Months)
			{
				case 1: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 2: if (Days > 29) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 3: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 4: if (Days > 30) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 5: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 6: if (Days > 30) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 7: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 8: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 9: if (Days > 30) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 10: if (Days > 31) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 11: if (Days > 30) {Days = 1; Months++; OnNewMonth.Broadcast(Months); } break;
				case 12: if (Days > 31) {Days = 1; Months=1; OnNewMonth.Broadcast(Months); } break;
				default: break;
			}
		}
	}
}

void UCentralTimeComponent::ExecuteCentralTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CentralTimerHandle);
	CentralTimerHandle.Invalidate();
	
	const float NormalizedTime = CentralDayTimeInSeconds / 1440.f;
	const float ScaledNormalizedTime = NormalizedTime / CentralTimeScale;
	
	GetWorld()->GetTimerManager().SetTimer(CentralTimerHandle, this, &UCentralTimeComponent::AdvanceTime, ScaledNormalizedTime, true);
}

