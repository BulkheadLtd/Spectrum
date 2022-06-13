// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetRealtimePlot.h"

// File picker requirements
#include <Developer/DesktopPlatform/Public/IDesktopPlatform.h>
#include <Developer/DesktopPlatform/Public/DesktopPlatformModule.h>

#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>

#include <Misc/FileHelper.h>

#include <imgui.h>

#include "SpectrumUtils.h"

USpectrumWidgetRealtimePlot::USpectrumWidgetRealtimePlot()
{
}

bool USpectrumWidgetRealtimePlot::BeginInternal(const UWorld* World)
{
	if (ImGui::Button(bIsPaused ? "Unpause" : "Pause"))
	{
		bIsPaused = !bIsPaused;
	}

	ImGui::SameLine();

	if (ImGui::Button("Save to CSV"))
	{
		if (GEngine)
		{
			if (GEngine->GameViewport)
			{
				const TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
				if (Window.IsValid())
				{
					const TSharedPtr<FGenericWindow> NativeWindow = Window->GetNativeWindow();
					if (NativeWindow.IsValid())
					{
						const void* ParentWindowHandle = NativeWindow->GetOSWindowHandle();
						if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
						{
							TArray<FString> OutFileNames;
							DesktopPlatform->SaveFileDialog(ParentWindowHandle, "Dump Data", ".", FString(""), "CSV|*.csv", 0, OutFileNames);

							if (OutFileNames.Num() > 0)
							{
								SaveToFile(OutFileNames[0]);
							}
						}
					}
				}
			}
		}
	}

	if (ImGui::Button("Toggle Auto Scale"))
	{
		bAutoScale = !bAutoScale;
	}	

	const FString PlotName = OverallPlotName != NAME_None ? OverallPlotName.ToString() : "";
	if (ImPlot::BeginPlot(TCHAR_TO_ANSI(*PlotName)))
	{
		static constexpr ImPlotAxisFlags Flags = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;

		const FString FinalAxisXName = AxisXName != NAME_None ? AxisXName.ToString() : "";
		const FString FinalAxisYName = AxisYName != NAME_None ? AxisYName.ToString() : "";
		ImPlot::SetupAxes(TCHAR_TO_ANSI(*FinalAxisXName),TCHAR_TO_ANSI(*FinalAxisYName), Flags);
		
		ImPlot::SetupAxesLimits(0,MaxSize,YAxisMinBound * (1 + YAxisMinDeadZone),YAxisMaxBound * (1 + YAxisMaxDeadZone), (bAutoScale) ? ImPlotCond_Always : ImPlotCond_Once);

		for (const TPair<FName, FSpectrumWidgetPlotInfo>& PlotInfoPair : Plots)
		{
			const FSpectrumWidgetPlotInfo& PlotInfo = PlotInfoPair.Value;
			const FName& PlotID = PlotInfo.PlotTitle;

			if (PlotInfo.bUseCustomColor)
			{
				ImPlot::PushStyleColor(ImPlotCol_Fill, PlotInfo.Color);
				ImPlot::PushStyleColor(ImPlotCol_Line, PlotInfo.Color);
			}

			// #TODO(): [07/03/2022] Downsampler for displaying dense plots without frames dipping

			// Render the filler
			if (bShowFill)
			{
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotShadedG(TCHAR_TO_ANSI(*PlotID.ToString()),
					&USpectrumRealtimePlotRingBuffer::ImPlotGetterFunc, (void*)PlotInfo.BufferUpper.Get(),
					&USpectrumRealtimePlotRingBuffer::ImPlotGetterFunc, (void*)PlotInfo.BufferLower.Get(),
					MaxSize);
				ImPlot::PopStyleVar();
			}

			// Render the line on top
			if (bShowLine)
			{
				ImPlot::PlotLineG(TCHAR_TO_ANSI(*PlotID.ToString()), &USpectrumRealtimePlotRingBuffer::ImPlotGetterFunc, (void*)PlotInfo.BufferUpper.Get(), MaxSize);
			}

			if (PlotInfo.bUseCustomColor)
			{
				ImPlot::PopStyleColor();
				ImPlot::PopStyleColor();
			}

			// Render tags
			int32 TagIndex = 0;
			for (TPair<FName, FTagInfo>& TagInfoPair : Tags)
			{
				FTagInfo& TagInfo = TagInfoPair.Value;

				ImPlot::DragLineY(TagIndex, &TagInfo.YPos, TagInfo.LineColor, 1, ImPlotDragToolFlags_NoInputs | ImPlotDragToolFlags_NoCursors);
				ImPlot::TagY(TagInfo.YPos, TagInfo.Color, TCHAR_TO_ANSI(*TagInfo.TagName.ToString()), "");

				TagIndex++;
			}
		}

		ImPlot::EndPlot();
	}

	return true;
}

void USpectrumWidgetRealtimePlot::EndInternal()
{
}

void USpectrumWidgetRealtimePlot::SetHistorySize(int32 NewSize)
{
	for (TPair<FName, FSpectrumWidgetPlotInfo>& PlotInfoPair : Plots)
	{
		FSpectrumWidgetPlotInfo& PlotInfo = PlotInfoPair.Value;

		if (IsValid(PlotInfo.BufferUpper) && IsValid(PlotInfo.BufferLower))
		{
			PlotInfo.BufferUpper->Reserve(NewSize);
			PlotInfo.BufferLower->Reserve(NewSize);
		}
	}

	MaxSize = NewSize;
}

void USpectrumWidgetRealtimePlot::Setup(int32 InHistorySize, float InInitialYZoom, const FName& InAxisXLabel, const FName& InAxisYLabel)
{
	SetHistorySize(InHistorySize);
	SetInitialYZoom(InInitialYZoom);
	SetAxisXLabel(InAxisXLabel);
	SetAxisYLabel(InAxisYLabel);
}

void USpectrumWidgetRealtimePlot::AddPlot(const FName& PlotName, const FName& PlotTitle)
{
	if (!Plots.Contains(PlotName))
	{
		FSpectrumWidgetPlotInfo& NewPlotInfo = Plots.Add(PlotName, FSpectrumWidgetPlotInfo());

		NewPlotInfo.PlotTitle = PlotTitle;

		// Create new buffers
		NewPlotInfo.BufferUpper = NewObject<USpectrumRealtimePlotRingBuffer>(this);
		NewPlotInfo.BufferLower = NewObject<USpectrumRealtimePlotRingBuffer>(this);

		if (IsValid(NewPlotInfo.BufferUpper) && IsValid(NewPlotInfo.BufferLower))
		{
			NewPlotInfo.BufferUpper->Reserve(MaxSize);
			NewPlotInfo.BufferLower->Reserve(MaxSize);
		}
	}
}

void USpectrumWidgetRealtimePlot::AddPlot(const FName& PlotName, const FName& PlotTitle, const FLinearColor& Color)
{
	AddPlot(PlotName, PlotTitle);
	if (Plots.Contains(PlotName))
	{
		if (FSpectrumWidgetPlotInfo* NewPlotInfo = Plots.Find(PlotName))
		{
			NewPlotInfo->bUseCustomColor = true;
			NewPlotInfo->Color = UEColorToImVec4(Color);
		}
	}
}

FSpectrumWidgetPlotInfo* USpectrumWidgetRealtimePlot::GetPlot(const FName& PlotName)
{
	FSpectrumWidgetPlotInfo* Result = nullptr;

	if (Plots.Contains(PlotName))
	{
		Result = &Plots[PlotName];
	}

	return Result;
}

void USpectrumWidgetRealtimePlot::AddTag(const FName& TagName, const float YPos, const FLinearColor& Color)
{
	FTagInfo& NewTagInfo = Tags.FindOrAdd(TagName);
	NewTagInfo.TagName   = TagName;
	NewTagInfo.YPos      = YPos;
	NewTagInfo.Color     = UEColorToImVec4(Color);
	NewTagInfo.LineColor = UEColorToImVec4(Color);
	NewTagInfo.LineColor.w = 0.3f; // Make line slightly less obnoxious
}

void USpectrumWidgetRealtimePlot::PushPoint(const FName& PlotName, float A)
{
	// If paused then don't allow pushing of points
	if (bIsPaused)
	{
		return;
	}

	// We only even push into the upper buffer, as we aren't doing anything fancy with the lower one in shaded mode
	if (FSpectrumWidgetPlotInfo* PlotInfo = Plots.Find(PlotName))
	{
		if (IsValid(PlotInfo->BufferUpper))
		{
			PlotInfo->BufferUpper->Add(A);

			if (bAutoScale)
			{
				SetYAxisBounds(FMath::Min(A, YAxisMinBound), FMath::Max(A, YAxisMaxBound));
			}
		}
	}
}

void USpectrumWidgetRealtimePlot::SaveToFile(const FString& FileName)
{
	// Generate string
	FString FileData = "";
	static const FString Separator = ";";

	// Header
	for (const TPair<FName, FSpectrumWidgetPlotInfo>& PlotInfoPair : Plots)
	{
		const FSpectrumWidgetPlotInfo& PlotInfo = PlotInfoPair.Value;
		FileData += PlotInfo.PlotTitle.ToString() + " [X]" + Separator + PlotInfo.PlotTitle.ToString() + " [Y]" + Separator;
	}

	FileData += "\n";

	// Data
	for (int32 i = 0; i < MaxSize; i++)
	{
		for (const TPair<FName, FSpectrumWidgetPlotInfo>& PlotInfoPair : Plots)
		{
			const FSpectrumWidgetPlotInfo& PlotInfo = PlotInfoPair.Value;

			if (!IsValid(PlotInfo.BufferUpper))
			{
				continue;
			}

			const ImPlotPoint Point = PlotInfo.BufferUpper->GetPoint(i);
			FileData += FString::FromInt(i) + Separator + FString::SanitizeFloat(Point.y) + Separator;
		}

		FileData += "\n";
	}


	FFileHelper::SaveStringToFile(FileData, *FileName);
}

void USpectrumWidgetRealtimePlot::SetTitle(const FName& NewTitle)
{
	OverallPlotName = NewTitle;
}

void USpectrumWidgetRealtimePlot::SetInitialYZoom(float NewZoom)
{
	YAxisMaxBound = NewZoom;
}

void USpectrumWidgetRealtimePlot::SetYAxisBounds(float NewMin, float NewMax)
{
	YAxisMaxBound = NewMax;
	YAxisMinBound = NewMin;
}

void USpectrumWidgetRealtimePlot::SetYAxisDeadZones(float MinDeadZone, float MaxDeadZone)
{
	YAxisMinDeadZone = MinDeadZone;
	YAxisMaxDeadZone = MaxDeadZone;
}

void USpectrumWidgetRealtimePlot::ToggleAutoScale(bool bToggle)
{
	bAutoScale = bToggle;
}

void USpectrumWidgetRealtimePlot::SetAxisYLabel(const FName& NewLabel)
{
	AxisYName = NewLabel;
}

void USpectrumWidgetRealtimePlot::SetAxisXLabel(const FName& NewLabel)
{
	AxisXName = NewLabel;
}
