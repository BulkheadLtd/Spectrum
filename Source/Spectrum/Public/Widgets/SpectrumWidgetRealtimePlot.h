// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include <implot.h>

#include "SpectrumRingBuffer.h"
#include "SpectrumWidgetBase.h"

#include "SpectrumWidgetRealtimePlot.generated.h"

USTRUCT()
struct FSpectrumWidgetPlotInfo
{
	GENERATED_BODY()

	FName PlotTitle = "";

	UPROPERTY()
	TObjectPtr<USpectrumRealtimePlotRingBuffer> BufferUpper = nullptr;

	UPROPERTY()
	TObjectPtr<USpectrumRealtimePlotRingBuffer> BufferLower = nullptr;

	bool bUseCustomColor = false;
	ImVec4 Color;
};

UCLASS()
class SPECTRUM_API USpectrumWidgetRealtimePlot : public USpectrumWidgetBase
{
	GENERATED_BODY()

	struct FTagInfo
	{
		FName TagName = "";
		double YPos = 0.f;
		ImVec4 Color = ImVec4();
		ImVec4 LineColor = ImVec4();
	};

public:

	USpectrumWidgetRealtimePlot();

	// Begin USpectrumWidgetBase overrides
	virtual bool BeginInternal(const UWorld* World) override;
	virtual void EndInternal() override;
	// End USpectrumWidgetBase overrides

	// Sets the maximum number of X points this plot can have. Effectively altering the history size
	virtual void SetHistorySize(int32 NewSize);

	virtual void Setup(int32 InHistorySize, float InInitialYZoom, const FName& InAxisXLabel, const FName& InAxisYLabel);

	// Adds a line plot to the overall plot
	virtual void AddPlot(const FName& PlotName, const FName& PlotTitle);
	virtual void AddPlot(const FName& PlotName, const FName& PlotTitle, const FLinearColor& Color);

	virtual FSpectrumWidgetPlotInfo* GetPlot(const FName& PlotName);

	// Adds a horizontal immovable tag onto the canvas at a specific Y position
	virtual void AddTag(const FName& TagName, const float YPos, const FLinearColor& Color);

	// Pushes a value into the buffer
	virtual void PushPoint(const FName& PlotName, float A);

	// Dumps all plots onto the disk
	virtual void SaveToFile(const FString& FileName);

	virtual void SetTitle(const FName& NewTitle);
	
	virtual void SetInitialYZoom(float NewZoom);
	virtual void SetYAxisBounds(float NewMin, float NewMax);
	virtual void SetYAxisDeadZones(float MinDeadZone, float MaxDeadZone);

	virtual void ToggleAutoScale(bool bToggle);
	
	virtual void SetAxisYLabel(const FName& NewLabel);
	virtual void SetAxisXLabel(const FName& NewLabel);

private:

	UPROPERTY()
	TMap<FName, FSpectrumWidgetPlotInfo> Plots;

	TMap<FName, FTagInfo> Tags;

	FName OverallPlotName = "";
	FName AxisXName = "";
	FName AxisYName = "";

	bool bShowLine = true;
	bool bShowFill = true;
	bool bIsPaused = false;
	
	float YAxisMaxBound = 100.f;
	float YAxisMinBound = 0.f;

	float YAxisMaxDeadZone = 0.f;
	float YAxisMinDeadZone = 0.f;

	bool bAutoScale = false;
	
	// The maximum X points this graph will show
	int32 MaxSize = 500;
};
