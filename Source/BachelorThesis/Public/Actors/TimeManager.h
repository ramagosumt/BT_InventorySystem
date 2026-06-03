#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

class UTimelineComponent;
class AMainGameState;
class ADirectionalLight;

UCLASS()
class BACHELORTHESIS_API ATimeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATimeManager();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UTimelineComponent* SunsetTimeline;
	
	UPROPERTY()
	AMainGameState* GameState;
	
	UPROPERTY( EditInstanceOnly )
	ADirectionalLight* DirectionalLight;
	
	UPROPERTY( EditAnywhere )
	UCurveFloat* SunMovement;
	
	UFUNCTION()
	void ExecuteSunrise();
	
	UFUNCTION()
	void ExecuteSunset();
	
	UFUNCTION()
	void SunriseStart(float Value);
	
	UFUNCTION()
	void SunsetStart(float Value);

};
