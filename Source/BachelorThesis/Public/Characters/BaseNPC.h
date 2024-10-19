#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "BaseNPC.generated.h"

#define ECC_NPC ECC_GameTraceChannel2

class UBaseInteractableComponent;
class UWidgetComponent;
class UTextRenderComponent;
class UGraphDialogueComponent;

UCLASS()
class BACHELORTHESIS_API ABaseNPC : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UBaseInteractableComponent* BaseInteractable;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UTextRenderComponent* NameComponent;

	UPROPERTY(EditDefaultsOnly, Category= "Defaults")
	UGraphDialogueComponent* DialogueComponent;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> SkeletalMesh;

public:
	ABaseNPC();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	FORCEINLINE void ShowWidgetComponent() const { WidgetComponent->SetHiddenInGame(false); }
	FORCEINLINE void HideWidgetComponent() const { WidgetComponent->SetHiddenInGame(true); }

protected:
	UPROPERTY(VisibleInstanceOnly, Category= "Defaults")
	FInteractionData NPCInteractionData;

	UPROPERTY(EditAnywhere)
	FName CharacterName;

	UPROPERTY(EditAnywhere, Category= "Defaults")
	float InteractionDuration;
	
	virtual void BeginPlay() override;

	void UpdateInteractionData();
	
};
