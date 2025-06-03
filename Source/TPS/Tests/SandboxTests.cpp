#if (WITH_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt,
                                 "TPSGame.Math.MaxInt",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt,
                                 "TPSGame.Math.Sqrt",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sqrt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

using namespace TPS::Test;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");

    using MaxIntTestPayload = TArray<TestPayload<TInterval<int32>, int32>>;
    const MaxIntTestPayload TestData {{{13, 25}, 25}, {{25, 25}, 25}, {{0, 123}, 123}, {{0, 0}, 0}, {{-1234, 0}, 0}, {{-47, -67}, -47}, {{-10, -10}, -10}, {{-123, 123}, 123}};

    for (const auto Data : TestData) { TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue); }

    // 13, 25
    // 25, 25
    // 0, 123
    // 0, 0
    // -1234, 0
    // -45, -67
    // -10, -10
    // -123, 123

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Sqrt function testing");

    using SqrtTestPayload = TArray<TestPayload<float, float>>;
    const SqrtTestPayload TestData {
        {4.0f, 2.0f, 0.0f}, {3.0f, 1.7f, 0.1f}, {3.0f, 1.73f, 0.01}, {3.0f, 1.7320f, 0.0001}, {16.0f, 4.0f, 0.0f}, {36.0f, 6.0f, 0.0f},
    };

    for (const auto Data : TestData)
    {
        FString str = FString::Printf(TEXT("Sqrt(%f) [%f]"), Data.TestValue, Data.ExpectedValue);
        TestEqual(str, FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);
    }

    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("Sin function testing");

    using Degrees = float;
    using SinTestPayload = TArray<TestPayload<Degrees, float>>;
    const SinTestPayload TestData {
        {Degrees {0.0f}, 0.0f}, {Degrees {30.0f}, 0.5f}, {Degrees {45.0f}, 0.707f}, {Degrees {60.0f}, 0.866f}, {Degrees {90.0f}, 1.0f},
    };

    for (const auto Data : TestData)
    {
        const float Rad = FMath::DegreesToRadians(Data.TestValue);
        FString str = FString::Printf(TEXT("Sqrt(%f) [%f]"), Data.TestValue, Data.ExpectedValue);
        TestEqual(str, FMath::Sin(Rad), Data.ExpectedValue, 0.001f);
    }

    return true;
}

#endif