// Constant.Inc


#include "Tests/Components/TPSInputRecordingComponent.h"
#include "EnhancedInputComponent.h"
#include "TPS/Tests/Utils/JsonUtils.h"

using namespace TPS::Test;

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    check(GetOwner());
    check(GetWorld());
    check(GetOwner()->InputComponent);

    EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetOwner()->InputComponent);
    check(EnhancedInputComponent);

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.BindingsData.Add(MakeBindingData());
}

void UTPSInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JsonUtils::WriteInputData(GenerateFileName(), InputData);
}

void UTPSInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.BindingsData.Add(MakeBindingData());
}

FBindingsData UTPSInputRecordingComponent::MakeBindingData() const
{
    FBindingsData BindingsData;
    BindingsData.WorldTime = GetWorld()->TimeSeconds;
    for (const auto& Binding : EnhancedInputComponent->GetActionEventBindings())
    {
        const auto ActionValue = EnhancedInputComponent->GetBoundActionValue(Binding->GetAction());
        BindingsData.InputActions.Add(FInputActionData {const_cast<UInputAction*>(Binding->GetAction()), ActionValue});
    }
    return BindingsData;
}

FString UTPSInputRecordingComponent::GenerateFileName() const
{
    FString SavedDir = FPaths::ProjectSavedDir();
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append("/Tests/").Append(FileName).Append("_").Append(Date).Append(".json");
}