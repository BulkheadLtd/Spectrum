// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumWindow.h"

#include <Engine/World.h>

#include <imgui.h>
#include <imgui_internal.h>

#include "SpectrumModule.h"
#include "SpectrumSettings.h"
#include "SpectrumWorldSubsystem.h"

USpectrumWindow::USpectrumWindow()
{
}

USpectrumWindow::USpectrumWindow(const FString& InTitle, const FString& InMainMenuTitle, const FString& InSubmenuEntryTitle)
	: Title(InTitle)
{
}

USpectrumWindow::~USpectrumWindow()
{
}

FString USpectrumWindow::GetTitle()
{
	return Title;
}

FVector2D USpectrumWindow::GetInitialSize()
{
	return { 200.f, 200.f };
}

void USpectrumWindow::ImGuiTick(float DeltaTime, UWorld* World)
{
	if (OnImGuiTick.IsBound())
	{
		OnImGuiTick.Execute();
	}

	if (bIsVisible)
	{
		BeginRespectWindowTransparencySetting();

		const bool bWasVisible = bIsVisible;

		// Set the window's size the first time it's created
		const FVector2D InitialSize = GetInitialSize();
		const ImVec2 InitialSizeImGui = { (float)InitialSize.X, (float)InitialSize.Y };
		ImGui::SetNextWindowSize(InitialSizeImGui, ImGuiCond_Once);

		const FString FullWindowName = GetTitle() + "###" + Guid.ToString();
		if (ImGui::Begin(TCHAR_TO_ANSI(*FullWindowName), &bIsVisible))
		{
			DrawWindowContent(DeltaTime, World);
			ImGui::End();
		}

		EndRespectWindowTransparencySetting();

		// Here we detect whether the user has pressed close. This helps us detect whether the window was created
		// but just not visible or whether the user has intentionally closed the window.
		if (bWasVisible && !bIsVisible && IsValid(World))
		{
			// Firstly, tell subsystem about this if applicable
			USpectrumWorldSubsystem* SpectrumSubsystem = World->GetSubsystem<USpectrumWorldSubsystem>();
			if (IsValid(SpectrumSubsystem))
			{
				SpectrumSubsystem->DestroyWindow(this);
			}
			else
			{
				// Start destroying self if not applicable
				ConditionalBeginDestroy();
			}
		}
	}
}

void USpectrumWindow::DrawWindowContent(float DeltaTime, UWorld* World)
{
	if (OnDrawWindowContent.IsBound())
	{
		OnDrawWindowContent.Execute();
	}
}

int32 USpectrumWindow::GetImGuiGuid() const
{
	return Guid.A + Guid.B + Guid.C + Guid.D;
}

void USpectrumWindow::SetIsVisible(bool bSetVisible)
{
	bIsVisible = bSetVisible;
}

bool USpectrumWindow::GetIsVisible()
{
	return bIsVisible;
}

bool* USpectrumWindow::GetIsVisiblePtr()
{
	return &bIsVisible;
}

bool* USpectrumWindow::GetIsCollapsedPtr()
{
	return &bIsCollapsed;
}

void USpectrumWindow::BeginRespectWindowTransparencySetting()
{
	int32 WindowTransparency = 100;

	// #TODO(): [10/03/2022] Perhaps cache this?
	// Get spectrum settings
	TObjectPtr<USpectrumSettings> SpectrumSettings = FSpectrumModule::GetSettings();
	if (IsValid(SpectrumSettings))
	{
		WindowTransparency = SpectrumSettings->WindowTransparency;
	}

	// Mod the existing window background color
	ImVec4 WindowBg;
	if (const ImGuiContext* Context = ImGui::GetCurrentContext())
	{
		WindowBg = Context->Style.Colors[ImGuiCol_WindowBg];
		WindowBg.w *= (static_cast<float>(WindowTransparency) / 100.f);
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, WindowBg);
}

void USpectrumWindow::EndRespectWindowTransparencySetting()
{
	ImGui::PopStyleColor();
}
