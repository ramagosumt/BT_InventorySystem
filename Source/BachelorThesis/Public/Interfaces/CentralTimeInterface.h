#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CentralTimeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCentralTimeInterface : public UInterface
{
	GENERATED_BODY()
};

class BACHELORTHESIS_API ICentralTimeInterface
{
	GENERATED_BODY()

public:
	virtual void PauseTime();
	virtual void ResumeTime();
	virtual void UpdateTimeScale(float NewScale);
	
};
