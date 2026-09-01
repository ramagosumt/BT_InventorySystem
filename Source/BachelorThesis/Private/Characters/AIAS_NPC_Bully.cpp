#include "Characters/AIAS_NPC_Bully.h"

#include "Controllers/AIAS_AIC_Bully.h"

AAIAS_NPC_Bully::AAIAS_NPC_Bully()
{
	// ===== Character =====
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BullyMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny"));
	if (BullyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(BullyMesh.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> BullyAnimInstance(TEXT("/Game/Rama_AIAS/Animations/ABP_Manny_Bully"));
	if (BullyAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BullyAnimInstance.Class);
	}
	
	AIControllerClass = AAIAS_AIC_Bully::StaticClass();
}
