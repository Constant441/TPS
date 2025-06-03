#if (WITH_AUTOMATION_TESTS)

#include "Tests/TestUtils.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "BufferVisualizationData.h"
#include "EnhancedInputComponent.h"
#include "Misc/OutputDeviceNull.h"

namespace TPS
{
    namespace Test
    {
        UWorld* TPS::Test::GetTestGameWorld()
        {
            const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
            for (const FWorldContext& Context : WorldContexts)
            {
                if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && (Context.World() != nullptr))
                {
                    return Context.World();
                }
            }
            return nullptr;
        }

        void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
        {
            if (!IsValid(Object))
                return;

            FString Command = FString::Printf(TEXT("%s"), *FuncName);
            for (const auto Param : Params)
            {
                Command.Append(" ").Append(Param);
                FOutputDeviceNull OutputDeviceNull;
                Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
            }
        }

        FTPSUntilLatentCommand::FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout) :
            Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
        {}

        bool FTPSUntilLatentCommand::Update()
        {
            const double NewTime = FPlatformTime::Seconds();
            if (NewTime - StartTime >= Timeout)
            {
                TimeoutCallback();
                return true;
            }

            Callback();
            return false;
        }

        int32 GetActionBindingIndexByName(const UEnhancedInputComponent* InputComponent, const FString& ActionName, ETriggerEvent TriggerEvent)
        {
            if (!IsValid(InputComponent))
                return INDEX_NONE;
            for (int32 i = 0; i < InputComponent->GetActionEventBindings().Num(); ++i)
            {
                const auto& Binding = InputComponent->GetActionEventBindings()[i];
                if (Binding->GetAction()->GetName().Equals(ActionName) && Binding->GetTriggerEvent() == TriggerEvent)
                {
                    return i;
                }
            }
            return INDEX_NONE;
        }

        UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
        {
            if (!IsValid(Widget) || !IsValid(Widget->WidgetTree))
                return nullptr;

            UWidget* FoundWidget = nullptr;
            UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget, [&](UWidget* Child) {
                if (IsValid(Child) && Child->GetName() == WidgetName)
                {
                    FoundWidget = Child;
                }
            });
            return FoundWidget;
        }

        void DoInputAction(const UEnhancedInputComponent* EnhancedInputComponent, const FString& ActionName, ETriggerEvent TriggerEvent)
        {
            if (!IsValid(EnhancedInputComponent))
                return;

            const int32 ActionIndex = GetActionBindingIndexByName(EnhancedInputComponent, ActionName, TriggerEvent);
            if (ActionIndex != INDEX_NONE)
            {
                const auto& Binding = EnhancedInputComponent->GetActionEventBindings()[ActionIndex];
                if (!Binding.IsValid())
                    return;
                Binding->Execute(FInputActionInstance(Binding->GetAction()));
            }
        }


        void JumpPressed(const UEnhancedInputComponent* EnhancedInputComponent)
        {
            DoInputAction(EnhancedInputComponent, "IA_Jump", ETriggerEvent::Started);
        }

        void PausePressed(const UEnhancedInputComponent* EnhancedInputComponent)
        {
            DoInputAction(EnhancedInputComponent, "IA_Pause", ETriggerEvent::Started);
        }

        bool FTakeGameScreenshotLatentCommand::Update()
        {
            if (!ScreenshotRequested)
            {
                const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
                UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetTestGameWorld(), ScreenshotName, FString {}, Options);
                ScreenshotRequested = true;
            }
            return CommandCompleted;
        }

        bool FTakeUIScreenshotLatentCommand::Update()
        {
            if (!ScreenshotSetupDone)
            {
                ScreenshotSetupDone = true;
                SetBufferVisualization("Opacity");
                return false;
            }
            if (!ScreenshotRequested)
            {
                const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
                UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetTestGameWorld(), ScreenshotName, Options);
                ScreenshotRequested = true;
            }
            return CommandCompleted;
        }

        void FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared()
        {
            FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
            SetBufferVisualization(NAME_None);
        };

        void FTakeUIScreenshotLatentCommand::SetBufferVisualization(const FName& VisualizeBuffer)
        {
            if (UGameViewportClient* ViewportClient = AutomationCommon::GetAnyGameViewportClient())
            {
                static IConsoleVariable* ICVar = IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
                if (ICVar)
                {
                    if (ViewportClient->GetEngineShowFlags())
                    {
                        ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(VisualizeBuffer == NAME_None ? false : true);
                        ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer == NAME_None ? true : false);
                        ICVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
                    }
                }
            }
        };
    }
}
#endif
