#if (WITH_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Science/ScienceFuncLib.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple,
                                 "TPSGame.Science.Fibonacci.Simple",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress,
                                 "TPSGame.Science.Fibonacci.Stress",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors,
                                 "TPSGame.Science.Fibonacci.LogHasErrors",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

DEFINE_SPEC(FFactorial, "TPSGame.Science.Factorial", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

DEFINE_SPEC(FFactorialStress, "TPSGame.Science.Factorial", EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority)

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci simple testing");

    // I: 0 1 2 3 4 5 6 7  8  9  10 ...
    // V: 0 1 1 2 3 5 8 13 21 34 55 ...

    const TArray<TPS::Test::TestPayload<int32, int32>> TestData {{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}};
    for (const auto Data : TestData)
    {
        TestTrueExpr(UScienceFuncLib::Fibonacci(Data.TestValue) == Data.ExpectedValue);

        const FString InfoString = FString::Printf(TEXT("test value %i, expected value %i"), Data.TestValue, Data.ExpectedValue);
        TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }

    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci stress testing");

    int32 PrevPrevValue = 0;
    int32 PrevValue = 1;

    for (int32 i = 2; i < 40; ++i)
    {
        const int32 CurrentValue = UScienceFuncLib::Fibonacci(i);
        TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }
    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci negative number on input produces error");

    AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::MatchType::Contains);
    UScienceFuncLib::Fibonacci(-10);

    return true;
}

void FFactorial::Define()
{
    Describe("Corner cases", [this]() {
        It("Factorial of 0 should be equal 1", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(0) == 1);
        });
        It("Factorial of 1 should be equal 1", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(1) == 1);
        });
        It("Factorial of negative number is undefined and func should return -1 (INDEX_None)", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(-21) == -1);
        });
    });

    Describe("Normal cases", [this]() {
        It("Factorial of 2 should be equal 2", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(2) == 2);
        });
        It("Factorial of 3 should be equal 3", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(3) == 6);
        });
        It("Factorial of 4 should be equal 4", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(4) == 24);
        });
        It("Factorial of 5 should be equal 5", [this]() {
            TestTrueExpr(UScienceFuncLib::Factorial(5) == 120);
        });

        using namespace TPS::Test;
        const TArray<TestPayload<int32, int32>> TestData {{5, 120}, {6, 720}, {7, 5040}, {8, 40320}};
        for (const auto& Data : TestData)
        {
            It(FString::Printf(TEXT("Factorial of %i should be equal %i"), Data.TestValue, Data.ExpectedValue), [this, Data]() {
                TestTrueExpr(UScienceFuncLib::Factorial(Data.TestValue) == Data.ExpectedValue);
            });
        }
    });
}

void FFactorialStress::Define()
{
    Describe("Factorial stress testing", [this]() {
        int32 PrevValue = 1;

        for (int32 i = 1; i <= 13; ++i)
        {
            const int32 CurrentValue = UScienceFuncLib::Factorial(i);
            const int32 ExpectedValue = i * PrevValue;

            It(FString::Printf(TEXT("Factorial of %d should be equal %d"), i, ExpectedValue), [&]() {
                TestTrueExpr(CurrentValue == ExpectedValue);
            });

            PrevValue = CurrentValue;
        }
    });
}
#endif
