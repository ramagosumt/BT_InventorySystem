#include "Actors/AIAS_AIP_SplinePath.h"

#include "Components/SplineComponent.h"

AAIAS_AIP_SplinePath::AAIAS_AIP_SplinePath()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);
}

int32 AAIAS_AIP_SplinePath::GetSplinePointCount() const
{
	return SplineComponent->GetNumberOfSplinePoints();
}

FVector AAIAS_AIP_SplinePath::GetSplinePointLocation(int32 PointIndex) const
{
	return SplineComponent->GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
}