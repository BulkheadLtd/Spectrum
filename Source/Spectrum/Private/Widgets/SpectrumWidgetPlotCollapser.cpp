// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetPlotCollapser.h"

#include <imgui.h>

#include "Widgets/SpectrumWidgetRealtimePlot.h"

bool USpectrumWidgetPlotCollapser::BeginInternal(const UWorld* World)
{
	// We need to fetch the "sub"plot from the bound plot to then fetch its upper buffer so we can display it in the
	// title of the collapsing header.
	TObjectPtr<USpectrumRealtimePlotRingBuffer> PlotRingBuffer = nullptr;
	if (RealtimePlot.IsValid())
	{
		const TObjectPtr<FSpectrumWidgetPlotInfo> PlotInfo = RealtimePlot->GetPlot(SummaryPlotName);
		if (PlotInfo != nullptr)
		{
			PlotRingBuffer = PlotInfo->BufferUpper;
		}
	}

	// Extract the necessary information from the ring buffer to construct a decent title
	FString FullTitle = TitlePrefix + ": ";
	if (IsValid(PlotRingBuffer))
	{
		FullTitle += FString::SanitizeFloat(PlotRingBuffer->GetLatestPoint().y);
	}

	bool bResult = false;

	// Now build the UI
	FullTitle += "###" + Guid.ToString();
	const bool bIsOpen = ImGui::CollapsingHeader(RAWCHARPTR(FullTitle));

	// If the tree node is open, then let the plot render
	if (bIsOpen && RealtimePlot.IsValid())
	{
		RealtimePlot->Begin(World);
		RealtimePlot->End();
	}
	
	bResult = bIsOpen;

	return bResult;
}

void USpectrumWidgetPlotCollapser::SetTitlePrefix(const FString& InTitle)
{
	TitlePrefix = InTitle;
}

void USpectrumWidgetPlotCollapser::BindPlot(TWeakObjectPtr<USpectrumWidgetRealtimePlot> InRealtimePlot, const FName& InPlotName)
{
	RealtimePlot = InRealtimePlot;
	SummaryPlotName = InPlotName;
}
