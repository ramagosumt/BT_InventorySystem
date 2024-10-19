#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/BaseItem.h"
#include "GridInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated)

class UBaseItem;

USTRUCT()
struct FLine
{
	GENERATED_BODY()

	FVector2D Start;
	FVector2D End;
};

USTRUCT()
struct FTile
{
	GENERATED_BODY()

	int32 X;
	int32 Y;
};

UENUM()
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName= "No Item Added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName= "Partial Amount Item Added"),
	IAR_AllItemAdded UMETA(DisplayName= "All Item Added")
};

USTRUCT()
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded)
	{};
	
	int32 ActualAmountAdded;
	
	EItemAddResult OperationResult;

	static FItemAddResult AddedNone()
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;

		return AddedNoneResult; 
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;

		return AddedPartialResult;
	};
	
	static FItemAddResult AddedAll(const int32 AmountAdded)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;

		return AddedAllResult;
	};

	bool operator==(const FItemAddResult& OtherAddResult) const
	{
		return this->ActualAmountAdded == OtherAddResult.ActualAmountAdded && this->OperationResult == OtherAddResult.OperationResult;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACHELORTHESIS_API UGridInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
	FVector2D InventorySize;

public:
	FOnInventoryUpdated OnInventoryUpdated;
	
	UGridInventoryComponent();
	
	TMap<UBaseItem*, FTile> GetAllItems();
	UBaseItem* GetItemAtIndex(const int32 Index) const;
	bool IsSpaceAvailable(const UBaseItem* InItem, const int32 TopLeftIndex) const;
	void RemoveSingleInstanceOfItem(const UBaseItem* OutItem);
	FItemAddResult HandleAddItem(UBaseItem* InItem);
	void AddNewItem(UBaseItem* InItem, const int32 TopLeftIndex, const int32 AmountToAdd);

	FORCEINLINE int32 GetSizeX() const { return InventorySize.X; }
	FORCEINLINE int32 GetSizeY() const { return InventorySize.Y; }

	FORCEINLINE void SetSizeX(const int32 NewX) { InventorySize.X = NewX; }
	FORCEINLINE void SetSizeY(const int32 NewY) { InventorySize.Y = NewY; }

	FORCEINLINE FTile IndexToTile(const int32 Index) const { return FTile(Index % GetSizeX(), Index / GetSizeX()); }
	FORCEINLINE int32 TileToIndex(const FTile Tile) const { return Tile.X + Tile.Y * GetSizeX(); }

protected:
	UPROPERTY(VisibleAnywhere, Category= "Defaults")
	TArray<UBaseItem*> InventoryContent;
	
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UBaseItem* InItem, const int32 AmountToAdd);
	int32 HandleStackableItems(UBaseItem* InItem, const int32 AmountToAdd);
	UBaseItem* FindNextPartialStack(UBaseItem* InItem) const;

	static int32 CalculateNumberForFullStack(const UBaseItem* InStackableItem, const int32 InitialAmountToAdd);
	
};
