// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include <implot.h>

#include "SpectrumRingBuffer.generated.h"

UCLASS()
class USpectrumRealtimePlotRingBuffer : public UObject
{
	GENERATED_BODY()

public:
	
	static ImPlotPoint ImPlotGetterFunc(void* UserData, int Index);

	void Reserve(int32 Size);
	
	ImPlotPoint GetPoint(int Index) const;
	ImPlotPoint GetLatestPoint() const;

	void Add(float A);

	const TArray<float>& GetData() const;
	int32 GetSize() const;

	float GetLocalizedPeak();
	float GetUltimatePeak() const;

private:
	
	TArray<float> Data;
	float UltimatePeak = 0.f;
	int32 WriteIndex = 0;

	bool bLocalizedPeakDirty = false;
	float CachedLocalizedPeak = 0.f;
};
