#pragma once

#if (WITH_AUTOMATION_TESTS)

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"

enum class ETriggerEvent : uint8;

namespace TPS
{
    namespace Test
    {
        template<typename Type1, typename Type2>
        struct TestPayload
        {
            Type1 TestValue;
            Type2 ExpectedValue;
            float Tolerance = UE_SMALL_NUMBER;
        };


#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 Index = 0; Index < StaticEnum<TYPE>()->NumEnums() - 1; ++Index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(StaticEnum<TYPE>()->GetValueByIndex(Index));
#define ENUM_LOOP_END }

        template<typename EnumType, typename FunctionType>
        void ForEach(FunctionType&& Function)
        {
            const UEnum* Enum = StaticEnum<EnumType>();
            for (int32 i = 0; i < Enum->NumEnums(); ++i) { Function(static_cast<EnumType>(Enum->GetValueByIndex(i))); }
        }


        template<typename T>
        T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
        {
            const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
            return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
        }

        template<typename T>
        T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
        {
            const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
            return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
        }

        class LevelScope
        {
        public:
            LevelScope(const FString& MapName) { AutomationOpenMap(MapName); };

            ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); };
        };

        UWorld* GetTestGameWorld();

        void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

        class FTPSUntilLatentCommand : public IAutomationLatentCommand
        {
        public:
            FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout = 5.0f);
            virtual bool Update() override;

        private:
            TFunction<void()> Callback;
            TFunction<void()> TimeoutCallback;
            float Timeout;
        };

        int32 GetActionBindingIndexByName(const UEnhancedInputComponent* InputComponent, const FString& ActionName, ETriggerEvent TriggerEvent);

        inline FString GetTestsDataDir()
        {
            return FPaths::GameSourceDir().Append("TPS/Tests/Data/");
        };

        template<typename T>
        T* FindWidgetByClass()
        {
            TArray<UUserWidget*> Widgets;
            UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetTestGameWorld(), Widgets, T::StaticClass(), false);
            return Widgets.IsEmpty() ? nullptr : Cast<T>(Widgets[0]);
        }

        UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName);

        void DoInputAction(const UEnhancedInputComponent* EnhancedInputComponent, const FString& ActionName, ETriggerEvent TriggerEvent);
        void JumpPressed(const UEnhancedInputComponent* EnhancedInputComponent);
        void PausePressed(const UEnhancedInputComponent* EnhancedInputComponent);


        class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
        {
        public:
            FTakeScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
            {
                FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
            };

            virtual ~FTakeScreenshotLatentCommand() { FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this); };

        protected:
            const FString ScreenshotName;
            bool ScreenshotRequested {false};
            bool CommandCompleted {false};

            virtual void OnScreenshotTakenAndCompared() { CommandCompleted = true; };
        };

        class FTakeGameScreenshotLatentCommand : public FTakeScreenshotLatentCommand
        {
        public:
            FTakeGameScreenshotLatentCommand(const FString& InScreenshotName) : FTakeScreenshotLatentCommand(InScreenshotName) {}

            virtual bool Update() override;
        };

        class FTakeUIScreenshotLatentCommand : public FTakeScreenshotLatentCommand
        {
        public:
            FTakeUIScreenshotLatentCommand(const FString& InScreenshotName) : FTakeScreenshotLatentCommand(InScreenshotName) {};

            virtual bool Update() override;

        private:
            bool ScreenshotSetupDone {false};

            virtual void OnScreenshotTakenAndCompared() override;
            void SetBufferVisualization(const FName& VisualizeBuffer);
        };
    }
}

#endif
