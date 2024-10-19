#include "BachelorThesis/Public/Frameworks/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "Characters/BachelorThesisCharacter.h"
#include "UIs/InteractionBarWidget.h"
#include "UIs/PlayerMenuWidget.h"
#include "Interfaces/InteractionInterface.h"

AMainHUD::AMainHUD() :
MainCharacter(nullptr), PlayerMenu(nullptr), InteractionBar(nullptr), bIsPlayerMenuOn(false), bIsCinematicModeOn(false)
{
	DesiredPlayerMenu = ConstructorHelpers::FClassFinder<UPlayerMenuWidget> (TEXT("/Game/Rama_InventorySystem/UIs/W_PlayerMenu")).Class;
	DesiredInteractionBar = ConstructorHelpers::FClassFinder<UInteractionBarWidget> (TEXT("/Game/Rama_InteractionSystem/UIs/W_InteractionBar")).Class;
	
	DesiredGridTileSize = 70.f;
}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<ABachelorThesisCharacter>(GetOwningPawn());
	
	PlayerMenu = CreateWidget<UPlayerMenuWidget>(GetOwningPlayerController(), DesiredPlayerMenu);
	if (MainCharacter)
	{
		if (PlayerMenu)
		{
			PlayerMenu->SetGridInventory(MainCharacter->GetGridInventory());
			PlayerMenu->SetSlotInventory(MainCharacter->GetSlotInventory());
			PlayerMenu->SetListInventory(MainCharacter->GetListInventory());
			PlayerMenu->SetTileSize(DesiredGridTileSize);
			PlayerMenu->AddToViewport();
			PlayerMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	InteractionBar = CreateWidget<UInteractionBarWidget>(GetOwningPlayerController(), DesiredInteractionBar);
	if (InteractionBar)
	{
		InteractionBar->AddToViewport();
		InteractionBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::ToggleInventoryMenu()
{
	if (PlayerMenu)
	{
		if (bIsCinematicModeOn)
		{
			return;
		}
		
		if (bIsPlayerMenuOn)
		{
			PlayerMenu->StopDragDrop();
			PlayerMenu->SetVisibility(ESlateVisibility::Collapsed);
			const FInputModeGameOnly GameOnly;
			GetWorld()->GetFirstPlayerController()->SetInputMode(GameOnly);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			GetWorld()->GetFirstPlayerController()->SetCinematicMode(false, true, true);
		}
		else
		{
			PlayerMenu->SetVisibility(ESlateVisibility::Visible);
			const FInputModeGameAndUI GameAndUI;
			GetWorld()->GetFirstPlayerController()->SetInputMode(GameAndUI);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetCinematicMode(true, true, true);
		}

		bIsPlayerMenuOn = !bIsPlayerMenuOn;
	}
}

void AMainHUD::UpdateInteractionBar(const FInteractionData& NewInteractionData) const
{
	if (InteractionBar)
	{
		InteractionBar->UpdateWidget(NewInteractionData);
		
		if (InteractionBar->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainHUD::ShowInteractionBar() const
{
	if (InteractionBar) InteractionBar->SetVisibility(ESlateVisibility::Visible);
}

void AMainHUD::HideInteractionBar() const
{
	if (InteractionBar) InteractionBar->SetVisibility(ESlateVisibility::Collapsed);
}
