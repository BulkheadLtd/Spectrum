// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWindow.h"

#include "SpectrumDebugWindowBase.generated.h"

// Self registering debug window class
UCLASS(Abstract)
class SPECTRUM_API USpectrumDebugWindowBase : public USpectrumWindow
{
	GENERATED_BODY()

public:

	virtual FName GetMenuTitle() const;
	virtual bool IsBuiltIn() const;
};
