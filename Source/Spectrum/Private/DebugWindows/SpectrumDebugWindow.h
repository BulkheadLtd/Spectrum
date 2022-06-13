// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <MultiGPU.h>

#include "SpectrumDebugWindowBase.h"

#include "SpectrumDebugWindow.generated.h"

struct FGameplayTagNode;

class USpectrumWidgetPlotCollapser;
class USpectrumWidgetFVector;
class USpectrumWidgetRealtimePlot;

UCLASS()
class SPECTRUM_API USpectrumDebugWindow : public USpectrumDebugWindowBase
{
	GENERATED_BODY()

public:

	USpectrumDebugWindow();

	// Begin USpectrumWindow override
	virtual void DrawWindowContent(float DeltaTime, UWorld* World) override;
	// End USpectrumWindow override

	// Begin USpectrumDebugWindowBase override
	virtual FName GetMenuTitle() const override;
	virtual bool IsBuiltIn() const override;
	// End USpectrumDebugWindowBase override

protected:

	void RenderGameplayTags(const UWorld* World);
	void RenderGameplayTagTree(const UWorld* World, const TSharedPtr<FGameplayTagNode> InTagNode);

	TMap<UClass*, int32> ActorCounts;

	UPROPERTY()
	TObjectPtr<USpectrumWidgetFVector> PlayerPosition = nullptr;

	UPROPERTY()
	TObjectPtr<USpectrumWidgetRealtimePlot> FrametimesPlot = nullptr;

	UPROPERTY()
	TObjectPtr<USpectrumWidgetPlotCollapser> FrametimePlotCollapser = nullptr;

	UPROPERTY()
	TObjectPtr<USpectrumWidgetRealtimePlot> DrawCallsPlot = nullptr;

	UPROPERTY()
	TObjectPtr<USpectrumWidgetPlotCollapser> DrawCallsPlotCollapser = nullptr;

	// Unit frame times filtered with a simple running average
	float RenderThreadTime;
	float GameThreadTime;
	float GPUFrameTime[MAX_NUM_GPUS];
	float FrameTime;
	float RHITTime;
	float InputLatencyTime;

	// Raw equivalents of the above variables
	float RawRenderThreadTime;
	float RawGameThreadTime;
	float RawGPUFrameTime[MAX_NUM_GPUS];
	float RawFrameTime;
	float RawRHITTime;
	float RawInputLatencyTime;

	// Time that has transpired since the last draw call
	double LastTime;

	bool bIncludeSpectrumFrametime = true;
};
