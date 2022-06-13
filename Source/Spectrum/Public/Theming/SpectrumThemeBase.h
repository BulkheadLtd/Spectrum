// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumThemeBase.generated.h"

struct ImGuiContext;

// Acts as the base theme. Any class that inherits this class will be automatically registered under the theme manager.
UCLASS(Abstract)
class SPECTRUM_API USpectrumThemeBase : public UObject
{
	GENERATED_BODY()

public:
	
	virtual FName GetThemeName() const;
	virtual void ApplyTheme(ImGuiContext* Context);
};
