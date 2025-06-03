// Constant.Inc

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"


UCLASS()
class TPS_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Fibonacci(int32 Value);

    /**
     * Calculates factorial for the input number
     * https://en.wikipedia.org/wiki/Factorial
     *
     * @param Value Number for which factorial should be calculated
     * @return      Factorial or (-1) for negative numbers
     */
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Factorial(int32 Value);
};
