#include "Controllers/AIAS_AIC_Student.h"

#include "StateTree.h"

AAIAS_AIC_Student::AAIAS_AIC_Student()
{
	// ===== StateTree =====

	static ConstructorHelpers::FObjectFinder<UStateTree> StudentStateTree(TEXT("/Game/Rama_AIAS/StateTrees/ST_Student_Root"));
	if (StudentStateTree.Succeeded())
	{
		SetStateTree(StudentStateTree.Object);
	}
}
