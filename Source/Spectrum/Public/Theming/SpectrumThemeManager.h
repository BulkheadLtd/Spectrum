// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "SpectrumThemeManager.generated.h"

class USpectrumThemeBase;
struct ImGuiContext;

UCLASS()
class SPECTRUM_API USpectrumThemeManager : public UObject
{
	GENERATED_BODY()

public:

	void Initialize();

	TArray<FName> GetAvailableThemeNames();
	FName GetCurrentTheme() const;

	// Tell the theme manager to notify everything needed to queue a theme change
	void RequestThemeChange(const FName& ThemeName, const bool bSave = true);

	// Actually apply the input theme to the input context. Does not inform settings manager and therefore does not save.
	void ApplyTheme(const FName& ThemeName, ImGuiContext* Context);
	void ApplyCurrentTheme(ImGuiContext* Context);

private:

	// Instanced list of all available themes. Reason we instance them and not just store their classes is because we
	// may want to do something to them to either permanently (via writing to disk) or temporarily modify them.
	// Basically gives us more flexibility down the road.
	UPROPERTY()
	TMap<FName, TObjectPtr<USpectrumThemeBase>> AvailableThemes;
};
