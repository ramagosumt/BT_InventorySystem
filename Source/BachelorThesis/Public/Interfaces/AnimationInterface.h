#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BACHELORTHESIS_API IAnimationInterface
{
	GENERATED_BODY()

public:
	virtual void BeginAnimation(USkeletalMeshComponent* Animation, FVector Location);
};
