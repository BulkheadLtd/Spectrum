// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWindow.h"

#include "SpectrumSettingsWindow.generated.h"

UCLASS()
class SPECTRUM_API USpectrumSettingsWindow : public USpectrumWindow
{
	GENERATED_BODY()

public:

	// Begin USpectrumWindow overrides
	virtual void DrawWindowContent(float DeltaTime, UWorld* World) override;
	virtual FString GetTitle() override;
	virtual FVector2D GetInitialSize() override;
	// End USpectrumWindow overrides
};
