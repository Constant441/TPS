// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSGameSetting.generated.h"

struct FSettingOption
{
    FText Name;   ///< Name of the option
    int32 Value;  ///< Value of the option
};

UCLASS()
class UTPSGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& InName);

    //! Sets an array options
    void SetOptions(const TArray<FSettingOption>& InOptions);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;

    //! \attention Func might be set before object using
    void AddGetter(TFunction<int32()> Func);

    //! \attention Func might be set before object using
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyPrevOption();
    void ApplyNextOption();

private:
    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    int32 GetCurrentValue() const;
    void SetCurrentValue(int32 Value);
    FORCENOINLINE int32 GetCurrentIndex();
};
