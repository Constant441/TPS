#include "Weapon/TPSTurret.h"
#include "Tests/AutomationTestSettings.h"
#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"

BEGIN_DEFINE_SPEC(FTurret, "TPSGame.Turret", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World = nullptr;
ATPSTurret* Turret = nullptr;

const char* MapName = "/Game/Test/EmptyTestLevel";
const char* TurretBPName = "Blueprint'/Game/Weapon/BP_TPSTurret.BP_TPSTurret'";
const char* TurretBPTestName = "Blueprint'/Game/Test/BP_Test_TPSTurret.BP_Test_TPSTurret'";
const FTransform InitialTransform = FTransform {FVector {1000.0f}};

END_DEFINE_SPEC(FTurret)

using namespace TPS::Test;

namespace
{
    void SpecCloseLevel(const UWorld* World)
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            PC->ConsoleCommand(TEXT("Exit"), true);
        }
    }

    template<typename ObjectClass, typename PropertyClass>
    PropertyClass GetPropertyValueByName(ObjectClass* Obj, const FString& PropName)
    {
        if (!Obj)
            return PropertyClass();
        for (TFieldIterator<FProperty> PropIt(Obj->StaticClass()); PropIt; ++PropIt)
        {
            const FProperty* Property = *PropIt;
            if (Property)
            {
                UE_LOG(LogTemp, Display, TEXT("%s"), *Property->GetName());
            }

            if (Property && Property->GetName().Equals(PropName))
            {
                return *Property->ContainerPtrToValuePtr<PropertyClass>(Obj);
            }
        }
        return PropertyClass();
    }
}

void FTurret::Define()
{
    Describe("Creational", [this]() {
        BeforeEach([this]() {
            AutomationOpenMap(MapName);
            World = GetTestGameWorld();
            TestNotNull("World exist", World);
        });

        It("Cpp instance can't created", [this]() {
            const FString ExpectedWarningMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSTurret::StaticClass()->GetName());
            AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::MatchType::Exact);


            Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
            TestNull("Turret item doesn't exist", Turret);
        });

        It("Blueprint instance can created", [this]() {
            const FTransform InitialTransform = FTransform {FVector {1000.0f}};
            Turret = CreateBlueprint<ATPSTurret>(World, TurretBPName, InitialTransform);
            TestNotNull("Turret exist", Turret);
        });


        AfterEach([this]() {
            SpecCloseLevel(World);
        });
    });

    Describe("Defaults", [this]() {
        BeforeEach([this]() {
            AutomationOpenMap(MapName);
            World = GetTestGameWorld();
            TestNotNull("World exist", World);

            Turret = CreateBlueprintDeferred<ATPSTurret>(World, TurretBPTestName, InitialTransform);
            TestNotNull("Turret exist", Turret);
        });

        const TArray<TTuple<int32, float>> TestData = {
            {45, 2.0f},
            {15, 3.0f},
            {5, 5.0f},
        };

        for (const auto& Data : TestData)
        {
            const auto TestName = FString::Printf(TEXT("Ammo: %i and fred: %.0f should be set up correctly"), Data.Key, Data.Value);

            It(TestName, [this, Data]() {
                const auto [Ammo, Freq] = Data;
                CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});

                Turret->FinishSpawning(InitialTransform);

                const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                TestTrueExpr(AmmoCount == Ammo);

                const int32 FireFrequency = GetPropertyValueByName<ATPSTurret, float>(Turret, "FireFrequency");
                TestTrueExpr(FireFrequency == Freq);
            });
        }

        AfterEach([this]() {
            SpecCloseLevel(World);
        });
    });

    Describe("Ammo", [this]() {
        const int32 InitialAmmoCount = 3;
        const float FireFreq = 2.0f;

        LatentBeforeEach([this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone) {
            AutomationOpenMap(MapName);
            World = GetTestGameWorld();
            TestNotNull("World exist", World);

            Turret = CreateBlueprintDeferred<ATPSTurret>(World, TurretBPTestName, InitialTransform);
            TestNotNull("Turret exist", Turret);

            CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(InitialAmmoCount), FString::SanitizeFloat(FireFreq)});
            Turret->FinishSpawning(InitialTransform);

            TestDone.Execute();
        });

        const auto TestName = FString::Printf(TEXT("Should be empty after %d seconds"), FMath::RoundToInt(InitialAmmoCount * FireFreq));

        LatentIt(TestName, EAsyncExecution::ThreadPool, FTimespan(0, 30.0f, 0), [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone) {
            AsyncTask(ENamedThreads::GameThread, [&]() {
                const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                TestTrueExpr(AmmoCount == InitialAmmoCount);

                const FTimerHandle FireTimerHandle = GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                TestTrueExpr(GetTestGameWorld()->GetTimerManager().IsTimerActive(FireTimerHandle));
            });

            const float SynchDelta = 0.5f;
            FPlatformProcess::Sleep(InitialAmmoCount * FireFreq + SynchDelta);

            AsyncTask(ENamedThreads::GameThread, [&]() {
                const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                TestTrueExpr(AmmoCount == 0);
                const FTimerHandle FireTimerHandle = GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                TestTrueExpr(!GetTestGameWorld()->GetTimerManager().IsTimerActive(FireTimerHandle));
            });

            TestDone.Execute();
        });

        LatentAfterEach([this](const FDoneDelegate& TestDone) {
            SpecCloseLevel(World);
            TestDone.Execute();
        });
    });
}


#endif
