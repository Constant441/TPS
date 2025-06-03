#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Settings/TPSGameUserSettings.h"
#include "TPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TPSGameUserSettingsTests,
                                 "TPSGame.GameUserSettings.SettingsShouldExist",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

using namespace TPS::Test;

bool TPSGameUserSettingsTests::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    TestTrueExpr(IsValid(UTPSGameUserSettings::Get()));
    TestTrueExpr(UTPSGameUserSettings::Get()->GetVideoSettings().Num() == 7);

    return true;
}

#endif
