#include "Characters/AIAS_NPC_Student.h"

#include "Controllers/AIAS_AIC_Student.h"

AAIAS_NPC_Student::AAIAS_NPC_Student()
{
	
	// ===== Character =====
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StudentMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny"));
	if (StudentMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(StudentMesh.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> StudentAnimInstance(TEXT("/Game/Rama_AIAS/Animations/ABP_Manny_Student"));
	if (StudentAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(StudentAnimInstance.Class);
	}
	
	AIControllerClass = AAIAS_AIC_Student::StaticClass();
}
