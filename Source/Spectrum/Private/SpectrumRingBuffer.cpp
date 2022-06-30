// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumRingBuffer.h"

#include <implot.h>

ImPlotPoint USpectrumRealtimePlotRingBuffer::ImPlotGetterFunc(void* UserData, int Index)
{
	ImPlotPoint Point;
	if (const USpectrumRealtimePlotRingBuffer* Buffer = static_cast<USpectrumRealtimePlotRingBuffer*>(UserData))
	{
		Point = Buffer->GetPoint(Index);
	}
	return Point;
}

void USpectrumRealtimePlotRingBuffer::Reserve(int32 Size)
{
	Data.Reset(Size);
	Data.Init(0, Size);
}

ImPlotPoint USpectrumRealtimePlotRingBuffer::GetPoint(int Index) const
{
	int32 FinalIndex = (WriteIndex + Index) % Data.Num();
		
	ImPlotPoint Point;
	Point.x = Index;

	if (Data.IsValidIndex(FinalIndex))
	{
		Point.y = Data[FinalIndex];
	}

	return Point;
}

ImPlotPoint USpectrumRealtimePlotRingBuffer::GetLatestPoint() const
{
	return GetPoint(Data.Num() - 1);
}

void USpectrumRealtimePlotRingBuffer::Add(float A)
{
	if (Data.Num() < 0 || !Data.IsValidIndex(WriteIndex))
	{
		// #TODO(): [04/03/2022] Error, not set up properly
		return;
	}
		
	Data[WriteIndex] = A;

	// Increment write index but don't let it overflow, instead just wrap
	WriteIndex++;
	if (WriteIndex >= Data.Num())
	{
		WriteIndex = 0;
	}

	if (A > UltimatePeak)
	{
		UltimatePeak = A;
	}

	bLocalizedPeakDirty = true;
}

const TArray<float>& USpectrumRealtimePlotRingBuffer::GetData() const
{
	return Data;
}

int32 USpectrumRealtimePlotRingBuffer::GetSize() const
{
	return Data.Num();
}

float USpectrumRealtimePlotRingBuffer::GetLocalizedPeak()
{
	// Recalculate localized peak if graph has changed
	// #TODO(): [09/03/2022] There must be a better way of doing this. This is a super naive approach. Please optimize!
	if (bLocalizedPeakDirty)
	{
		bLocalizedPeakDirty = false;
		CachedLocalizedPeak = INT_FAST32_MIN;
		
		for (int32 i = 0; i < Data.Num(); i++)
		{
			if (Data[i] > CachedLocalizedPeak)
			{
				CachedLocalizedPeak = i;
			}
		}
	}
	
	return CachedLocalizedPeak;
}

float USpectrumRealtimePlotRingBuffer::GetUltimatePeak() const
{
	return UltimatePeak;
}
