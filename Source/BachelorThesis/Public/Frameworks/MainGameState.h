#pragma once

#include "CoreMinimal.h"
#include "Interfaces/CentralTimeInterface.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

class UCentralTimeComponent;

/**
 * 
 */
UCLASS()
class BACHELORTHESIS_API AMainGameState : public AGameStateBase, public ICentralTimeInterface
{
	GENERATED_BODY()
	
public:
	AMainGameState();
	
	virtual void PauseTime() override;
	virtual void ResumeTime() override;
	virtual void UpdateTimeScale(float NewScale) override;
	
	FORCEINLINE UCentralTimeComponent* GetCentralTimeComponent() const { return CentralTimeComponent; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UCentralTimeComponent* CentralTimeComponent;
	
	UFUNCTION()
	void OnNewMonth(int32 CurrentMonthCount);
	
	UFUNCTION()
	void OnNewDay(int32 CurrentDayCount);
	
	UFUNCTION()
	void OnNewHour(int32 CurrentHourCount);
	
	UFUNCTION()
	void OnNewMinute(int32 CurrentMinuteCount);
	
};
