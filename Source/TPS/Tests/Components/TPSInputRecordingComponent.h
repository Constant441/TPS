// Constant.Inc

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Tests/Utils/InputRecordingUtils.h"
#include "TPSInputRecordingComponent.generated.h"

class UEnhancedInputComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInputRecordingComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly)
    FString FileName {"CharacterTestInput"};

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UEnhancedInputComponent* EnhancedInputComponent;

    FInputData InputData;

    FBindingsData MakeBindingData() const;
    FString GenerateFileName() const;
};
