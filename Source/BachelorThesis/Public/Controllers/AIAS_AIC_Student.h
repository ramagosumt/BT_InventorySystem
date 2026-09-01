#pragma once

#include "CoreMinimal.h"
#include "Controllers/AIAS_AIC.h"

#include "AIAS_AIC_Student.generated.h"

class UStateTreeAIComponent;

/** AIAS controller specialization for student behavior. */
UCLASS()
class BACHELORTHESIS_API AAIAS_AIC_Student : public AAIAS_AIC
{
	GENERATED_BODY()
	
public:
	AAIAS_AIC_Student();
};
