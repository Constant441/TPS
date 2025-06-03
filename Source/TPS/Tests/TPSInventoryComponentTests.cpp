#if (WITH_AUTOMATION_TESTS)

#include "Components/TPSInventoryComponent.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated,
                                 "TPSGame.Components.Inventory.ComponentCouldBeCreated",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZeroByDefault,
                                 "TPSGame.Components.Inventory.ItemScoresShouldBeZeroByDefault",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreShouldBeAdded,
                                 "TPSGame.Components.Inventory.NegativeScoreShouldBeAdded",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded,
                                 "TPSGame.Components.Inventory.PositiveScoreShouldBeAdded",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCanBeAdded,
                                 "TPSGame.Components.Inventory.ScoreMoreThanLimitCanBeAdded",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

using namespace TPS::Test;

namespace
{
    class UTPSInventoryComponentTestable : public UTPSInventoryComponent
    {
    public:
        void SetLimits(const TMap<EPickupType, int32>& Limits) { InventoryLimits = Limits; };
    };

    TMap<EPickupType, int32> InitLimits(UTPSInventoryComponentTestable* InvComp, int32 LimitValue)
    {
        TMap<EPickupType, int32> InvLimits;
        ENUM_LOOP_START(EPickupType, PickupElem)
        InvLimits.Add(PickupElem, LimitValue);
        ENUM_LOOP_END

        InvComp->SetLimits(InvLimits);

        return InvLimits;
    };
}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    AddInfo("Inventory component exist testing");
    const UTPSInventoryComponent* InvComp = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull("Inventory component exist", InvComp))
        return false;

    return true;
}

bool FItemScoresShouldBeZeroByDefault::RunTest(const FString& Parameters)
{
    AddInfo("Item score testing");
    const UTPSInventoryComponent* InvComp = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull("Inventory component exist", InvComp))
        return false;

    ForEach<EPickupType>([&](EPickupType EElem) {
        TestTrueExpr(InvComp->GetInventoryAmountByType(EElem) == 0);
    });


    return true;
}

bool FNegativeScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    AddInfo("Negative score added testing");
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp))
        return false;

    InitLimits(InvComp, 100);

    const int32 NegativeScore = -5;
    const int32 InitialScoreAmount = 10;

    ENUM_LOOP_START(EPickupType, PickupElem)
    TestTrueExpr(InvComp->TryToAddItem({PickupElem, InitialScoreAmount}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(PickupElem) == 10);

    TestTrueExpr(!InvComp->TryToAddItem({PickupElem, NegativeScore}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(PickupElem) == 10);
    ENUM_LOOP_END

    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    AddInfo("Positive score added testing");
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp))
        return false;

    const auto InvLimits = InitLimits(InvComp, 100);

    ENUM_LOOP_START(EPickupType, PickupElem)
    for (int32 i = 0; i < InvLimits[PickupElem]; ++i)
    {
        TestTrueExpr(InvComp->TryToAddItem({PickupElem, 1}));
        TestTrueExpr(InvComp->GetInventoryAmountByType(PickupElem) == i + 1);
    }
    ENUM_LOOP_END

    return true;
}

bool FScoreMoreThanLimitCanBeAdded::RunTest(const FString& Parameters)
{
    AddInfo("Negative score added testing");
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp))
        return false;

    int32 ScoreLimit = 5;
    InitLimits(InvComp, ScoreLimit);

    TestTrueExpr(InvComp->TryToAddItem({EPickupType::EPT_SPHERE, ScoreLimit}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EPickupType::EPT_SPHERE) == ScoreLimit);

    TestTrueExpr(!InvComp->TryToAddItem({EPickupType::EPT_SPHERE, ScoreLimit + 1}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EPickupType::EPT_SPHERE) == ScoreLimit);

    return true;
}

#endif
