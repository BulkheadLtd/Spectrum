// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>

#include "Theming/SpectrumThemeManager.h"

class USpectrumSettings;

DECLARE_DELEGATE(FSpectrumWindowDrawSignature);

class SPECTRUM_API FSpectrumModule : public IModuleInterface
{
public:

	// Begin IModuleInterface overrides
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	// End IModuleInterface overrides

	/// Singleton-like access to this module's interface. This is just for convenience!
	/// Beware of calling this during the shutdown phase, though. Your module might have been unloaded already.
	/// @return Returns singleton instance, loading the module on demand if needed
	static FSpectrumModule& Get();

	/// Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
	/// @return True if the module is loaded and ready to use
	static bool IsAvailable();

	static TObjectPtr<USpectrumSettings> GetSettings();
	TObjectPtr<USpectrumSettings> GetSpectrumSettings();
	
	TObjectPtr<USpectrumThemeManager> GetThemeManager();

private:

	// Spectrum settings
	UPROPERTY()
	TObjectPtr<USpectrumSettings> SpectrumSettings = nullptr;

	// Theme manager
	UPROPERTY()
	TObjectPtr<USpectrumThemeManager> ThemeManager = nullptr;
};
