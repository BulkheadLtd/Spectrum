// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumSettingsWindow.h"

#include <Engine/World.h>

#include <imgui.h>

#include "SpectrumModule.h"
#include "SpectrumSettings.h"
#include "Theming/SpectrumThemeManager.h"
#include "SpectrumUtils.h"

void USpectrumSettingsWindow::DrawWindowContent(float DeltaTime, UWorld* World)
{
	if (!IsValid(World) || !FSpectrumModule::IsAvailable())
	{
		return;
	}

	TObjectPtr<USpectrumSettings> SpectrumSettings = FSpectrumModule::Get().GetSpectrumSettings();
	if (!IsValid(SpectrumSettings))
	{
		return;
	}

	bool bSettingsRequireSaving = false;

	if (ImGui::Checkbox("Always Show Menu Bar", &SpectrumSettings->bAlwaysShowMenuBar))
	{
		bSettingsRequireSaving = true;
	}
	// Tooltip for the above
	IMGUI_SIMPLE_TOOLTIP("If disabled, then the menu bar will never show when Spectrum\nis not in focus (all windows will still continue to show)");

	ImGui::SliderInt("Window Transparency", &SpectrumSettings->WindowTransparency, 0, 100, "%d", ImGuiSliderFlags_None);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		bSettingsRequireSaving = true;
	}

	// Theme selector dropdown
	const TObjectPtr<USpectrumThemeManager> ThemeManager = FSpectrumModule::Get().GetThemeManager();
	if (IsValid(ThemeManager))
	{
		const FName CurrentThemeName = ThemeManager->GetCurrentTheme();
		if (ImGui::BeginCombo("Theme", RAWCHARPTR(CurrentThemeName.ToString())))
		{
			TArray<FName> AvailableThemeNames = ThemeManager->GetAvailableThemeNames();
			
			for (const FName& ThemeName : AvailableThemeNames)
			{
				if (ImGui::Selectable(RAWCHARPTR(ThemeName.ToString()), false))
				{
					ThemeManager->RequestThemeChange(ThemeName);
				}
			}
			
			ImGui::EndCombo();
		}
	}

	if (ImGui::InputInt("Max DebugDrawString Count", &SpectrumSettings->DebugDrawStringMaxCount))
	{
		bSettingsRequireSaving = true;
	}

	if (bSettingsRequireSaving)
	{
		SpectrumSettings->Save();
	}
}

FString USpectrumSettingsWindow::GetTitle()
{
	return "Spectrum Settings";
}

FVector2D USpectrumSettingsWindow::GetInitialSize()
{
	return { 500.f, 200.f };
}
