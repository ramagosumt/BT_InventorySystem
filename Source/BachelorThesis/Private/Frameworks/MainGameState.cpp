#include "Frameworks/MainGameState.h"
#include "Components/CentralTimeComponent.h"

AMainGameState::AMainGameState()
{
	CentralTimeComponent = CreateDefaultSubobject<UCentralTimeComponent>("CentralTimeComponent");
}

void AMainGameState::PauseTime()
{
	ICentralTimeInterface::PauseTime();
	
	CentralTimeComponent->PauseTime();
}

void AMainGameState::ResumeTime()
{
	ICentralTimeInterface::ResumeTime();
	
	CentralTimeComponent->ResumeTime();
}

void AMainGameState::UpdateTimeScale(float NewScale)
{
	ICentralTimeInterface::UpdateTimeScale(NewScale);
	
	CentralTimeComponent->UpdateTimeScale(NewScale);
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	
	CentralTimeComponent->OnNewDay.AddDynamic(this, &AMainGameState::OnNewDay);
}

void AMainGameState::OnNewMonth(int32 CurrentMonthCount)
{
	UE_LOG(LogTemp, Warning, TEXT("CentralTimeInterface::OnNewDay()"));
}

void AMainGameState::OnNewDay(int32 CurrentDayCount)
{
	UE_LOG(LogTemp, Warning, TEXT("CentralTimeInterface::OnNewDay()"));
}

void AMainGameState::OnNewHour(int32 CurrentHourCount)
{
	UE_LOG(LogTemp, Warning, TEXT("CentralTimeInterface::OnNewHour()"));
}

void AMainGameState::OnNewMinute(int32 CurrentMinuteCount)
{
	UE_LOG(LogTemp, Warning, TEXT("CentralTimeInterface::OnNewMinute()"));
}