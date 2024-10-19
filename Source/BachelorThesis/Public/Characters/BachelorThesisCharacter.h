#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BachelorThesisCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGridInventoryComponent;
class USlotInventoryComponent;
class UListInventoryComponent;
class UTraceInteractionComponent;
class AMainHUD;
class AGrid3DInventory;
struct FInputActionValue;
class UDataTable;

#define ECC_InventoryItem ECC_GameTraceChannel1

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABachelorThesisCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UGridInventoryComponent* GridInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USlotInventoryComponent* SlotInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UListInventoryComponent* ListInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UTraceInteractionComponent* TraceInteraction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchInventoryDimensionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LMBAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;

	UPROPERTY()
	AMainHUD* MainHUD;

	UPROPERTY()
	AGrid3DInventory* InventoryBag;

	bool bIsInteracting;

	bool bIs3D;

	UPROPERTY()
	UDataTable* DesiredInventoryData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> CommonItemIDs;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> QuestItemIDs;

	void InitializeInventories();
	void InitializeQuestInventory();
	void InitializeCommonInventory();
	void InitializeJournalInventory();

	void SpawnInventoryBag();

public:
	ABachelorThesisCharacter();
	
	virtual void Tick(float DeltaTime) override;

	void BeginInteract();
	void EndInteract();
	void ResetInteract();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UCameraComponent* GetFPCamera() const { return FPCamera; }
	FORCEINLINE UGridInventoryComponent* GetGridInventory() const { return GridInventory; }
	FORCEINLINE USlotInventoryComponent* GetSlotInventory() const { return SlotInventory; }
	FORCEINLINE UListInventoryComponent* GetListInventory() const { return ListInventory; }
	FORCEINLINE AMainHUD* GetMainHUD() const { return MainHUD; }
	
	FORCEINLINE void SetIsInteracting(const bool NewState) { bIsInteracting = NewState; }

protected:

	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleInventory();
	void SwitchInventoryDimension();
	void LMB();
	void IsLMBing();
	void StopLMB();
	void RotateItem();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
};
