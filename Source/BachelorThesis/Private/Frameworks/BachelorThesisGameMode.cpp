#include "BachelorThesis/Public/Frameworks/BachelorThesisGameMode.h"
#include "BachelorThesis/Public/Characters/BachelorThesisCharacter.h"
#include "Frameworks/MainController.h"
#include "Frameworks/MainHUD.h"
#include "UObject/ConstructorHelpers.h"

ABachelorThesisGameMode::ABachelorThesisGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AMainController::StaticClass();
	HUDClass = AMainHUD::StaticClass();
}
