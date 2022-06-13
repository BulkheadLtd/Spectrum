// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Theming/SpectrumThemeManager.h"

#include "SpectrumBuiltInThemes.h"
#include "SpectrumWorldSubsystem.h"
#include "Theming/SpectrumThemeBase.h"
#include "SpectrumSettings.h"

void USpectrumThemeManager::Initialize()
{
	// Go off and find all available theme classes
	TArray<UClass*> AvailableThemeClasses;
	GetDerivedClasses(USpectrumThemeBase::StaticClass(), AvailableThemeClasses, true);

	// Create each class in situ, adding them to the list of available themes as theme instances
	for (const UClass* AvailableThemeClass : AvailableThemeClasses)
	{
		if (!IsValid(AvailableThemeClass))
		{
			continue;
		}

		TObjectPtr<USpectrumThemeBase> ThemeInstance = NewObject<USpectrumThemeBase>(this, AvailableThemeClass);
		if (IsValid(ThemeInstance))
		{
			AvailableThemes.Add(ThemeInstance->GetThemeName(), ThemeInstance);
		}
	}

	// Nab the "default" theme, make an instance and read the name of it
	FName DefaultThemeName = NAME_None;
	if (IsValid(USpectrumThemeDefault::StaticClass()))
	{
		if (const USpectrumThemeDefault* QuickInstance = Cast<USpectrumThemeDefault>(USpectrumThemeDefault::StaticClass()->GetDefaultObject()))
		{
			DefaultThemeName = QuickInstance->GetThemeName();
		}
	}

	// Tell spectrum settings about our default theme name (and set it) if we don't have one set. Reason we do this here
	// is because on first setup, we read theme name from settings.
	const TObjectPtr<USpectrumSettings> SpectrumSettings = USpectrumSettings::Get();
    if (IsValid(SpectrumSettings))
    {
    	if (SpectrumSettings->ThemeName == NAME_None)
    	{
    		SpectrumSettings->ThemeName = DefaultThemeName;
    	}
    }
}

TArray<FName> USpectrumThemeManager::GetAvailableThemeNames()
{
	TArray<FName> Result;

	for (const TPair<FName, TObjectPtr<USpectrumThemeBase>>& ThemeInstancePair : AvailableThemes)
	{
		Result.Add(ThemeInstancePair.Key);
	}

	return Result;
}

FName USpectrumThemeManager::GetCurrentTheme() const
{
	FName Result = NAME_None;
	
	const TObjectPtr<USpectrumSettings> SpectrumSettings = USpectrumSettings::Get();
	if (IsValid(SpectrumSettings))
	{
		Result = SpectrumSettings->ThemeName;
	}
	
	return Result;
}

void USpectrumThemeManager::RequestThemeChange(const FName& ThemeName, const bool bSave)
{
	if (AvailableThemes.Contains(ThemeName))
	{
		// We now tell every Spectrum world context that we've changed theme
		USpectrumWorldSubsystem::ExecForEach([](USpectrumWorldSubsystem* SpectrumWorldSubsystem)
		{
			if (IsValid(SpectrumWorldSubsystem))
			{
				SpectrumWorldSubsystem->RequestThemeReload();
			}
		});

		// Tell settings to save this choice to disk
		const TObjectPtr<USpectrumSettings> SpectrumSettings = USpectrumSettings::Get();
		if (IsValid(SpectrumSettings))
		{
			SpectrumSettings->ThemeName = ThemeName;
			SpectrumSettings->Save();
		}
	}
}

void USpectrumThemeManager::ApplyTheme(const FName& ThemeName, ImGuiContext* Context)
{
	if (AvailableThemes.Contains(ThemeName))
	{
		TObjectPtr<USpectrumThemeBase> Theme = AvailableThemes.FindRef(ThemeName);
		if (IsValid(Theme))
		{
			Theme->ApplyTheme(Context);
		}
	}
}

void USpectrumThemeManager::ApplyCurrentTheme(ImGuiContext* Context)
{
	const FName CurrentTheme = GetCurrentTheme();
	if (AvailableThemes.Contains(CurrentTheme))
	{
		TObjectPtr<USpectrumThemeBase> Theme = AvailableThemes.FindRef(CurrentTheme);
		if (IsValid(Theme))
		{
			Theme->ApplyTheme(Context);
		}
	}
}
