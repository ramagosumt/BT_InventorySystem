#include "Components/DialogueComponent.h"
#include "Frameworks/MainHUD.h"
#include "Characters/AIAS_NPC.h"
#include "Kismet/GameplayStatics.h"
#include "UIs/DialogueWidget.h"

UDialogueComponent::UDialogueComponent():
	DialogueRow(FName("1")), DialogueWidget(nullptr)
{
	DesiredDialogueWidget = ConstructorHelpers::FClassFinder<UDialogueWidget> (TEXT("/Game/Rama_DialogueSystem/UIs/W_Dialogue")).Class;
}

void UDialogueComponent::Interact(ABachelorThesisCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);
	
	if (bIsDialogueEnd)
	{
		if (DialogueWidget) DialogueWidget->RemoveFromParent();
		
		bIsDialogueEnd = false;
		DialogueWidget = nullptr;
		
		HandleCinematic(false);
		
		OnDialogueEnd.Broadcast();
		
		return;
	}
	
	if (!DialogueWidget)
	{
		if (const FDialogueData* DialogueData = DialogueDataTable->FindRow<FDialogueData>(DialogueRow, DialogueRow.ToString()))
		{
			HandleUI(DialogueData);
			HandleCinematic(true);
			UpdateDialogueWidget(DialogueData);
		}
	}
	else
	{
		if (const FDialogueData* DialogueData = DialogueDataTable->FindRow<FDialogueData>(DialogueRow, DialogueRow.ToString()))
		{
			UpdateDialogueWidget(DialogueData);
		}
	}
}

void UDialogueComponent::SelectChoiceDialogue(const int32 ChoiceIndex)
{
	if (const FDialogueData* DialogueData = DialogueDataTable->FindRow<FDialogueData>(DialogueRow, DialogueRow.ToString()))
	{
		DialogueRow = DialogueData->ChoiceData[ChoiceIndex].NextRow;
		
		DialogueData = DialogueDataTable->FindRow<FDialogueData>(DialogueRow, DialogueRow.ToString());
		UpdateDialogueWidget(DialogueData);
	}
}

void UDialogueComponent::HandleUI(const FDialogueData* DialogueData)
{
	if (DesiredDialogueWidget)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DesiredDialogueWidget);
		if (DialogueWidget)
		{
			DialogueWidget->SetOwningDialogueComponent(this);
			DialogueWidget->UpdateWidget(FCString::Atoi(*DialogueRow.ToString()), FText::FromName(CharacterName), DialogueData->CurrentDialogue, TArray<FText>());
			DialogueWidget->AddToViewport();
		}
	}
}

void UDialogueComponent::HandleCinematic(const bool bCinematicMode) const {
	if (const auto MainController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (bCinematicMode)
		{
			const FInputModeGameAndUI InputMode;
			MainController->SetInputMode(InputMode);
		}
		else
		{
			const FInputModeGameOnly InputMode;
			MainController->SetInputMode(InputMode);
		}
		
		MainController->SetShowMouseCursor(bCinematicMode);
		MainController->SetCinematicMode(bCinematicMode, true, true);

		if (const auto MainHUD = Cast<AMainHUD>(MainController->GetHUD()))
		{
			if (bCinematicMode) MainHUD->HideInteractionBar();
				else MainHUD->ShowInteractionBar();
			
			MainHUD->SetCinematicMode(bCinematicMode);
		}

		if (const AAIAS_NPC* Owner = Cast<AAIAS_NPC>(GetOwner()))
			bCinematicMode ? Owner->HideInteractionWidget() : Owner->ShowInteractionWidget();
	}
}

void UDialogueComponent::UpdateDialogueWidget(const FDialogueData* DialogueData)
{
	TArray<FText> ChoiceArray;
	if (DialogueData->bHasChoice)
	{
		ChoiceArray.Reserve(DialogueData->ChoiceData.Num());
		for (const FDialogueChoiceData& Choice : DialogueData->ChoiceData)
		{
			ChoiceArray.Add(FText::FromName(Choice.ChoiceDialogue));
		}
	}
    
	DialogueWidget->UpdateWidget(
		FCString::Atoi(*DialogueRow.ToString()),
		FText::FromName(CharacterName),
		DialogueData->CurrentDialogue,
		ChoiceArray
	);

	DialogueRow = DialogueData->NextRow;
	bIsDialogueEnd = DialogueData->bIsDialogueEnd;
}