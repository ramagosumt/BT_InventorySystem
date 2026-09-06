#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAS_AIP_SplinePath.generated.h"

class USplineComponent;

UCLASS()
class BACHELORTHESIS_API AAIAS_AIP_SplinePath : public AActor
{
	GENERATED_BODY()
	
public:
	AAIAS_AIP_SplinePath();
	
	// ===== Pathing =====
	
	int32 GetSplinePointCount() const;
	FVector GetSplinePointLocation(int32 PointIndex) const;
	
private:
	// ===== Pathing =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USplineComponent> SplineComponent;
	
	int32 CurrentSplinePointIndex = 0;
	int32 SplineDirection = 1;
};
