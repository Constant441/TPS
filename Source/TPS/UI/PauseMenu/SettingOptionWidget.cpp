// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/SettingOptionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Settings/TPSGameSetting.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingDisplayName);
    check(SettingCurrentValue);
    check(PrevSettingButton);
    check(NextSettingButton);

    PrevSettingButton->OnClicked.AddDynamic(this, &USettingOptionWidget::OnPrevSetting);
    NextSettingButton->OnClicked.AddDynamic(this, &USettingOptionWidget::OnNextSetting);
}

void USettingOptionWidget::Init(UTPSGameSetting* InSetting)
{
    Setting = MakeWeakObjectPtr(InSetting);
    check(Setting.IsValid());

    UpdateTexts();
}

void USettingOptionWidget::UpdateTexts()
{
    if (Setting.IsValid())
    {
        SettingDisplayName->SetText(Setting->GetName());
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnNextSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyNextOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnPrevSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyPrevOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}
