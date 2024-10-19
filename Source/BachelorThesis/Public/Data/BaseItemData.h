#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	Common UMETA(DisplayName = "Common"),
	Quest UMETA(DisplayName = "Quest"),
	Journal UMETA(DisplayName = "Journal")
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	FItemNumericData() :
	MaxStackSize(0), bIsStackable(false)
	{}

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	FItemAssetData() :
	Icon(nullptr), Mesh(nullptr), Dimensions(0, 0)
	{};

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	FIntPoint Dimensions;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	FItemData() :
	Type(EItemType::Common)
	{};

	UPROPERTY(EditAnywhere, Category= "ItemData")
	FName ID;

	UPROPERTY(EditAnywhere, Category= "ItemData")
	EItemType Type;

	UPROPERTY(EditAnywhere, Category= "ItemData")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category= "ItemData")
	FItemNumericData NumericData;
	
	UPROPERTY(EditAnywhere, Category= "ItemData")
	FItemAssetData AssetData;
};
