// Copyright BULKHEAD Limited. All Rights Reserved.
	
#include "SpectrumModule.h"

#include "SpectrumSettings.h"

IMPLEMENT_MODULE(FSpectrumModule, Spectrum)

void FSpectrumModule::StartupModule()
{
	// Initialize the settings instance first
	SpectrumSettings = NewObject<USpectrumSettings>();
	if (IsValid(SpectrumSettings))
	{
		// Stop GC from cleaning house
		SpectrumSettings->AddToRoot();
	}

	// Create the theme manager
	ThemeManager = NewObject<USpectrumThemeManager>();
	if (IsValid(ThemeManager))
	{
		// Need to call AddToRoot here so GC doesn't come along and clean it up
		ThemeManager->AddToRoot();
		ThemeManager->Initialize();
	}
}

void FSpectrumModule::ShutdownModule()
{
	if (IsValid(ThemeManager))
	{
		ThemeManager->RemoveFromRoot();
	}

	if (IsValid(SpectrumSettings))
	{
		SpectrumSettings->RemoveFromRoot();
	}
}

bool FSpectrumModule::SupportsDynamicReloading()
{
	return true;
}

FSpectrumModule& FSpectrumModule::Get()
{
	return FModuleManager::LoadModuleChecked<FSpectrumModule>("Spectrum");
}

bool FSpectrumModule::IsAvailable()
{
	return FModuleManager::Get().IsModuleLoaded("Spectrum");
}

TObjectPtr<USpectrumSettings> FSpectrumModule::GetSettings()
{
	return Get().GetSpectrumSettings();
}

TObjectPtr<USpectrumSettings> FSpectrumModule::GetSpectrumSettings()
{
	return SpectrumSettings;
}

TObjectPtr<USpectrumThemeManager> FSpectrumModule::GetThemeManager()
{
	return ThemeManager;
}
