#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
    EPT_SPHERE = 0 UMETA(DisplayName = "Sphere"),
    EPT_CUBE = 1 UMETA(DisplayName = "Cube"),
    EPT_CYLINDER = 2 UMETA(DisplayName = "Cylinder"),
    EPT_CONE = 3 UMETA(DisplayName = "Cone"),
};

UCLASS()
class TPS_API UTPSLocalizationFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    static FText GetInventoryItemText(EPickupType ItemType)
    {
        switch (ItemType)
        {
            case EPickupType::EPT_SPHERE: return NSLOCTEXT("Inventory", "Sphere_Loc", "SPHERE");
            case EPickupType::EPT_CONE: return NSLOCTEXT("Inventory", "Cone_Loc", "CONE");
            case EPickupType::EPT_CUBE: return NSLOCTEXT("Inventory", "Cube_Loc", "CUBE");
            case EPickupType::EPT_CYLINDER: return NSLOCTEXT("Inventory", "Cylinder_Loc", "CYLINDER");
        }

        checkNoEntry();
        return FText();
    };
};

USTRUCT(BlueprintType)
struct FPickupData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    EPickupType Type = EPickupType::EPT_SPHERE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "0", ClampMax = "10000"))
    int32 Score = 0;

    FString ToString() const { return FString::Printf(TEXT("(Type=%s,Score=%i)"), *UEnum::GetValueAsString(Type), Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0"))
    float MaxHealth {100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0"))
    float HealModifier {10.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.01", Units = "s"))
    float HealRate {0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", Units = "s"))
    float LifeSpan {5.0f};

    FString ToString() const { return FString::Printf(TEXT("(MaxHealth=%f,HealModifier=%f,HealRate=%f,LifeSpan=%f)"), MaxHealth, HealModifier, HealRate, LifeSpan); }
};
