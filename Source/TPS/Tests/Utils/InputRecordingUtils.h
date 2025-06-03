// Constant.Inc

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputRecordingUtils.generated.h"

UENUM(BlueprintType)
enum class ESerializedInputActionValueType : uint8
{
    Boolean,
    Axis1D,
    Axis2D,
    Axis3D
};

USTRUCT(BlueprintType)
struct FSerializableInputActionValue
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Value = FVector::ZeroVector;

    UPROPERTY()
    ESerializedInputActionValueType ValueType = ESerializedInputActionValueType::Boolean;

    FSerializableInputActionValue() = default;

    FSerializableInputActionValue(const FInputActionValue& In)
    {
        switch (In.GetValueType())
        {
            case EInputActionValueType::Boolean:
                Value = FVector(In.Get<bool>() ? 1.f : 0.f, 0.f, 0.f);
                ValueType = ESerializedInputActionValueType::Boolean;
                break;
            case EInputActionValueType::Axis1D:
                Value = FVector(In.Get<float>(), 0.f, 0.f);
                ValueType = ESerializedInputActionValueType::Axis1D;
                break;
            case EInputActionValueType::Axis2D:
                const FVector2D Vec = In.Get<FVector2D>();
                Value = FVector(Vec.X, Vec.Y, 0.f);
                ValueType = ESerializedInputActionValueType::Axis2D;
                break;
            case EInputActionValueType::Axis3D:
                Value = In.Get<FVector>();
                ValueType = ESerializedInputActionValueType::Axis3D;
                break;
        }
    }

    FInputActionValue ToInputActionValue() const
    {
        switch (ValueType)
        {
            case ESerializedInputActionValueType::Boolean: return FInputActionValue(!Value.IsZero());
            case ESerializedInputActionValueType::Axis1D: return FInputActionValue(static_cast<float>(Value.X));
            case ESerializedInputActionValueType::Axis2D: return FInputActionValue(FVector2D(Value.X, Value.Y));
            case ESerializedInputActionValueType::Axis3D: return FInputActionValue(Value);
            default: return FInputActionValue(false);
        }
    }
};


USTRUCT()
struct FInputActionData
{
    GENERATED_BODY()

    UPROPERTY()
    UInputAction* Action;

    UPROPERTY()
    FSerializableInputActionValue Value;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FInputActionData> InputActions;

    UPROPERTY()
    float WorldTime {0.0f};
};

USTRUCT()
struct FInputData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FBindingsData> BindingsData;

    UPROPERTY()
    FTransform InitialTransform;
};
