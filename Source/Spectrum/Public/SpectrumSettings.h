// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumSettings.generated.h"

UCLASS(Config = Spectrum)
class SPECTRUM_API USpectrumSettings : public UObject
{
	GENERATED_BODY()
	
public:

	// Convenience method
	static TObjectPtr<USpectrumSettings> Get();

	void OnWorldBegin(UWorld* World);

	// Here mostly to remind devs to save after changes!
	// This is also called when this object goes out of scope.
	void Save();

	UPROPERTY(Config)
	bool bAlwaysShowMenuBar = false;

	UPROPERTY(Config)
	int32 WindowTransparency = 100;

	// Theme manager reads this directly
	UPROPERTY(Config)
	FName ThemeName = NAME_None;

	UPROPERTY(Config)
	int32 DebugDrawStringMaxCount = 0;
};
