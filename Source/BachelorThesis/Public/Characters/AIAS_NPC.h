#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"

#include "AIAS_NPC.generated.h"

#define ECC_NPC ECC_GameTraceChannel3

class ABachelorThesisCharacter;
class AAIAS_AIP_SplinePath;
class UBaseInteractableComponent;
class UDialogueComponent;
class UTextRenderComponent;
class UWidgetComponent;

/** Gaits describe physical movement rather than behavioral intent. */
UENUM(BlueprintType)
enum class EAIAS_NPC_MovementGait : uint8
{
	Sneak,
	Walk,
	Run,
	Sprint
};

/** Movement-speed configuration used by AIAS NPC characters. */
USTRUCT(BlueprintType)
struct FAIAS_NPC_MovementProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float SneakSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float WalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float RunSpeed = 350.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float SprintSpeed = 500.f;
};

/**
 * Base character for AIAS-controlled NPCs. 
 * 
 * Provides shared physical configuration, identity, movement, interaction, and dialogue.
 */
UCLASS()
class BACHELORTHESIS_API AAIAS_NPC : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AAIAS_NPC();
	
	// ===== Movement =====
	
	/**
	 * Applies the configured movement speed associated with the requested gait.
	 *
	 * @param NewGait Physical movement gait to apply.
	 */
	void SetMovementGait(EAIAS_NPC_MovementGait NewGait);
	
	// ===== Pathing =====
	
	void AdvanceSplinePoint();
	
	bool HasSplinePath() const;
	
	AAIAS_AIP_SplinePath* GetSplinePath() const;
	FVector GetCurrentSplinePointLocation() const;
	
	// ===== Interaction =====
	
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ABachelorThesisCharacter* PlayerCharacter) override;

protected:
	// ===== Actor =====
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
private:
	// ===== Identity =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> NameTextComponent;

	UPROPERTY(EditAnywhere, Category = "AIAS|Identity")
	FName CharacterName;

	// ===== Movement =====

	UPROPERTY(EditDefaultsOnly, Category = "AIAS|Movement")
	FAIAS_NPC_MovementProfile MovementProfile;
	
	// ===== Pathing =====
	
	UPROPERTY(EditInstanceOnly, Category = "AIAS|Movement")
	TObjectPtr<AAIAS_AIP_SplinePath> SplinePath;
	
	int32 CurrentSplinePointIndex = 0;
	int32 SplineDirection = 1;

	// ===== Interaction =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBaseInteractableComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "AIAS|Interaction")
	FInteractionData NPCInteractionData;

	UPROPERTY(EditDefaultsOnly, Category = "AIAS|Interaction", meta = (ClampMin = "0.0", Units = "s"))
	float InteractionDuration = 0.f;
	
	void UpdateInteractionData();
	
	// ===== Dialogue =====

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIAS|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDialogueComponent> DialogueComponent;
};
