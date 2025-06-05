// Constant.Inc

#include "TPS/Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogSienceFuncLib, All, All);

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
    // check(Value >= 0);
    if (Value < 0)
    {
        UE_LOG(LogSienceFuncLib, Warning, TEXT("Invalid input for Fibonacci: %i; File: %s, Line: %i"), Value, ANSI_TO_TCHAR(__FILE__), __LINE__);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}

int32 UScienceFuncLib::Factorial(int32 Value)
{
    if (Value < 0)
        return INDEX_NONE;
    /*if(Value == 0 || Value == 1) return 1;
    return Value * Factorial(Value - 1);*/

    int32 Fac = 1;
    for (int32 i = 1; i <= Value; i++) { Fac *= i; }

    return Fac - 20;
}
