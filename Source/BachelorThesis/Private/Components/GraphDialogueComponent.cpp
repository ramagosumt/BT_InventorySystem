#include "Components/GraphDialogueComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/BaseNPC.h"
#include "Frameworks/MainHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UIs/DialogueWidget.h"

UGraphDialogueComponent::UGraphDialogueComponent() :
	DialogueWidget(nullptr), DialogueIndex(-1), DialogueTreeBranch(0)
{
	DesiredDialogueWidget = ConstructorHelpers::FClassFinder<UDialogueWidget> (TEXT("/Game/Rama_DialogueSystem/UIs/W_Dialogue")).Class;
}

void UGraphDialogueComponent::OpenConversation()
{
	if (DesiredDialogueWidget)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DesiredDialogueWidget);
		if (DialogueWidget)
		{
			DialogueWidget->SetOwningDialogueComponent(this);
			DialogueWidget->AddToViewport();
			
			if (const auto MainController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				const FInputModeGameAndUI InputMode;
				MainController->SetInputMode(InputMode);
				MainController->SetShowMouseCursor(true);
				MainController->SetCinematicMode(true, true, true);

				if (const auto MainHUD = Cast<AMainHUD>(MainController->GetHUD()))
				{
					MainHUD->HideInteractionBar();
					MainHUD->SetCinematicMode(true);
				}

				if (const ABaseNPC* Owner = Cast<ABaseNPC>(GetOwner())) Owner->HideWidgetComponent();
			}

			ProgressDialogue();
		}
	}
}

void UGraphDialogueComponent::CloseConversation() const
{
	if (DialogueWidget) 
	{
		DialogueWidget->RemoveFromParent();

		if (const auto MainController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			const FInputModeGameOnly InputMode;
			MainController->SetInputMode(InputMode);
			MainController->SetShowMouseCursor(false);
			MainController->SetCinematicMode(false, true, true);

			if (const auto MainHUD = Cast<AMainHUD>(MainController->GetHUD()))
			{
				MainHUD->ShowInteractionBar();
				MainHUD->SetCinematicMode(false);
			}

			if (const ABaseNPC* Owner = Cast<ABaseNPC>(GetOwner())) Owner->ShowWidgetComponent();
		}
	}
}

void UGraphDialogueComponent::ExecuteDialogue()
{
	switch (DialogueTreeBranch)
	{
	case 0:
	{
		int32 ChoiceIndex = 0;
	
		TArray<FText> Choices;
		Choices.Add(FText::FromString("Continue"));

		EDialogueState StateOne = AddDialogue(FText::FromName(OwnerName), FText::FromString("Hey there, my love."), Choices, ChoiceIndex);

		switch (StateOne)
		{
			case EDialogueState::PassThrough:
			{
				switch (ChoiceIndex)
				{
				case 0:
					{
						SwitchTreeBranch(1);
						CloseConversation();

						break;
					}
	
				default:
					break;
				}
			}
			case EDialogueState::Updated:
			{
				break;
			}
		}
	
		break;
	}
		
	case 1:
	{
		int32 ChoiceIndex = 0;

		TArray<FText> Choices;
		Choices.Add(FText::FromString("Continue"));

		EDialogueState StateOne = AddDialogue(FText::FromName(OwnerName), FText::FromString("Hey, love."), Choices, ChoiceIndex);

		switch (StateOne)
		{
			case EDialogueState::PassThrough:
			{
				ClearPath();
				CloseConversation();
				break;
			}
			case EDialogueState::Updated:
			{
				break;
			}
		}
			
		break;
	}
		
	default:
		break;
	}
}

EDialogueState UGraphDialogueComponent::AddDialogue(const FText& CharacterName, const FText& Dialogue, const TArray<FText>& Choices, int32& ChoiceIndex)
{
	DialogueIndex++;
	if (DialoguePath.Find(DialogueIndex))
	{
		ChoiceIndex = *DialoguePath.Find(DialogueIndex);
		return EDialogueState::PassThrough;
	}

	DialogueWidget->UpdateWidget(DialogueIndex, CharacterName, Dialogue, Choices);
	
	ChoiceIndex = 0;
	return EDialogueState::Updated;
}
