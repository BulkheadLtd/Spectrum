// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumDebugWindow.h"

#include <EngineUtils.h>
#include <GameplayTagsManager.h>
#include <RHI.h>

#include <imgui.h>

#include "SpectrumModule.h"
#include "SpectrumWorldSubsystem.h"
#include "Widgets/SpectrumWidgetPlotCollapser.h"
#include "Widgets/SpectrumWidgetRealtimePlot.h"

USpectrumDebugWindow::USpectrumDebugWindow()
{
	Title = "Spectrum Debugger";

	FrametimesPlot = CreateDefaultSubobject<USpectrumWidgetRealtimePlot>("FrametimesPlot");
	if (IsValid(FrametimesPlot))
	{
		FrametimesPlot->SetHistorySize(1000);
		FrametimesPlot->SetInitialYZoom(40);
		FrametimesPlot->AddTag("33ms", 33.f, FColor::Red);
		FrametimesPlot->AddTag("16ms", 16.66f, FColor::Yellow);
		
		FrametimesPlot->SetAxisYLabel("Milliseconds");
		FrametimesPlot->AddPlot("Frame", "Frame");
		FrametimesPlot->AddPlot("Game Thread", "Game Thread");
		FrametimesPlot->AddPlot("Render Thread", "Render Thread");
		FrametimesPlot->AddPlot("GPU", "GPU");
	}

	FrametimePlotCollapser = CreateDefaultSubobject<USpectrumWidgetPlotCollapser>("FrametimePlotCollapser");
	if (IsValid(FrametimePlotCollapser))
	{
		FrametimePlotCollapser->BindPlot(FrametimesPlot, "Frame");
		FrametimePlotCollapser->SetTitlePrefix("Frametimes");
	}
	
	DrawCallsPlot = CreateDefaultSubobject<USpectrumWidgetRealtimePlot>("DrawCallsPlot");
	if (IsValid(DrawCallsPlot))
	{
		DrawCallsPlot->Setup(1000, 1000, "Draw Calls", "");
		DrawCallsPlot->AddPlot("Draw Calls", "Draw Calls");
	}

	DrawCallsPlotCollapser = CreateDefaultSubobject<USpectrumWidgetPlotCollapser>("DrawCallsPlotCollapser");
	if (IsValid(FrametimePlotCollapser))
	{
		DrawCallsPlotCollapser->BindPlot(DrawCallsPlot, "Draw Calls");
		DrawCallsPlotCollapser->SetTitlePrefix("Draw Calls");
	}
}

void USpectrumDebugWindow::DrawWindowContent(float DeltaTime, UWorld* World)
{
	float LastSpectrumDeltaTimeMs = 0.f;
	const TObjectPtr<USpectrumWorldSubsystem> SpectrumWorldSubsystem = World->GetSubsystem<USpectrumWorldSubsystem>();
	if (IsValid(SpectrumWorldSubsystem))
	{
		LastSpectrumDeltaTimeMs = SpectrumWorldSubsystem->GetLastDeltaTimeMs();
	}

	float DiffTime;
	if (FApp::IsBenchmarking() || FApp::UseFixedTimeStep())
	{
		// If we're in fixed time step mode, FApp::GetCurrentTime() will be incorrect for benchmarking
		const double CurrentTime = FPlatformTime::Seconds();
		if (LastTime == 0)
		{
			LastTime = CurrentTime;
		}
		DiffTime = CurrentTime - LastTime;
		LastTime = CurrentTime;
	}
	else
	{
		// Use the DiffTime we computed last frame, because it correctly handles the end of frame idling and corresponds better to the other unit times
		DiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	}
	
	RawFrameTime = DiffTime * 1000.0f;
	RawFrameTime -= bIncludeSpectrumFrametime ? 0.f : LastSpectrumDeltaTimeMs;
	FrameTime = 0.9 * FrameTime + 0.1 * RawFrameTime;

	// Number of milliseconds the gamethread was used last frame
	RawGameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);
	RawGameThreadTime -= bIncludeSpectrumFrametime ? 0.f : LastSpectrumDeltaTimeMs;
	GameThreadTime = 0.9 * GameThreadTime + 0.1 * RawGameThreadTime;

	// Number of milliseconds the renderthread was used last frame
	RawRenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);
	RenderThreadTime = 0.9 * RenderThreadTime + 0.1 * RawRenderThreadTime;

	for (uint32 GPUIndex : FRHIGPUMask::All())
	{
		// Number of milliseconds the GPU was busy last frame
		const uint32 GPUCycles = RHIGetGPUFrameCycles(GPUIndex);
		RawGPUFrameTime[GPUIndex] = FPlatformTime::ToMilliseconds(GPUCycles);
		GPUFrameTime[GPUIndex] = 0.9 * GPUFrameTime[GPUIndex] + 0.1 * RawGPUFrameTime[GPUIndex];
	}
	
	// Frame times section
	ImGui::Checkbox("Include Spectrum Frametime", &bIncludeSpectrumFrametime);
	
	FrametimesPlot->PushPoint("Frame", FrameTime);
	FrametimesPlot->PushPoint("Game Thread", GameThreadTime);
	FrametimesPlot->PushPoint("Render Thread", RenderThreadTime);
	FrametimesPlot->PushPoint("GPU", GPUFrameTime[0]);

	FrametimePlotCollapser->Begin(World);
	FrametimePlotCollapser->End();
	
	// Draw calls plot
	DrawCallsPlot->PushPoint("Draw Calls", GNumDrawCallsRHI[0]);
	
	DrawCallsPlotCollapser->Begin(World);
	DrawCallsPlotCollapser->End();

	if (IsValid(World))
	{
		ImGui::Text("World Name: %s", TCHAR_TO_ANSI(*World->GetMapName()));
	}
	
	if (ImGui::CollapsingHeader("Actor counts", ImGuiTreeNodeFlags_None))
	{
		// First clear all the counts
		for (TPair<UClass*, int>& ActorCountPair : ActorCounts)
		{
			ActorCountPair.Value = 0;
		}

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			const AActor* Actor = *It;
			if (IsValid(Actor))
			{
				int32& ActorCount = ActorCounts.FindOrAdd(Actor->GetClass());
				ActorCount++;
			}
		}

		// Reorder by count
		ActorCounts.ValueStableSort([](const int32& A, const int32& B)
		{
			return A > B;
		});
		
		static ImGuiTableFlags flags =
					ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
					| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
					| ImGuiTableFlags_ScrollY;
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		if (ImGui::BeginTable("table_sorting", 2, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * (ActorCounts.Num() + 1)), 0.0f))
		{
			static int32 ColumnID_Name  = 1;
			static int32 ColumnID_Count = 2;
			
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Name);
			ImGui::TableSetupColumn("Count", 0,0.0f, ColumnID_Count);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			for (const TPair<UClass*, int>& ActorCountPair : ActorCounts)
			{
				if (IsValid(ActorCountPair.Key))
				{
					ImGui::PushID(ActorCountPair.Key);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(TCHAR_TO_ANSI(*GetNameSafe(ActorCountPair.Key)));
					ImGui::TableNextColumn();
					ImGui::Text("%d", ActorCountPair.Value);
					ImGui::PopID();
				}
			}
			
			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Gameplay Tags", ImGuiTreeNodeFlags_None))
	{
		RenderGameplayTags(World);
	}
}

FName USpectrumDebugWindow::GetMenuTitle() const
{
	return FName("General");
}

bool USpectrumDebugWindow::IsBuiltIn() const
{
	return true;
}

void USpectrumDebugWindow::RenderGameplayTags(const UWorld* World)
{
#if WITH_EDITOR
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	
	TArray<TSharedPtr<FGameplayTagNode>> TagItems;
	GameplayTagsManager.GetFilteredGameplayRootTags(FString(), TagItems);
	
	for (const TSharedPtr<FGameplayTagNode> TagNode : TagItems)
	{
		RenderGameplayTagTree(World, TagNode);
	}
#endif
}

void USpectrumDebugWindow::RenderGameplayTagTree(const UWorld* World, const TSharedPtr<FGameplayTagNode> InTagNode)
{
	if (!InTagNode.IsValid())
	{
		return;
	}
	
	TArray<TSharedPtr<FGameplayTagNode>> Children = InTagNode->GetChildTagNodes();

	// Need to sort based on whether the nodes have children or not so that the ones that do have children rise to the top of the tree
	Children.Sort([](const TSharedPtr<FGameplayTagNode>& Lhs, const TSharedPtr<FGameplayTagNode>& Rhs) -> bool
	{
		if (Lhs.IsValid() && Rhs.IsValid())
		{
			return Lhs->GetChildTagNodes().Num() > Rhs->GetChildTagNodes().Num();
		}

		return false;
	});
	
	// Render param entry first
	if (Children.Num() > 0)
	{
		if (ImGui::TreeNode(TCHAR_TO_ANSI(*InTagNode->GetCompleteTagName().ToString())))
		{
			// Render children
			for (const TSharedPtr<FGameplayTagNode>& TagNode : Children)
			{
				RenderGameplayTagTree(World, TagNode);
			}
			
			ImGui::TreePop();
		}
	}
	else
	{
		ImGui::TextUnformatted(TCHAR_TO_ANSI(*InTagNode->GetCompleteTagName().ToString()));
	}
}
