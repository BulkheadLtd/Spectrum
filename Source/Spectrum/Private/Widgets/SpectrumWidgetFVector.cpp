// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetFVector.h"

#include <HAL/PlatformApplicationMisc.h>

#include <imgui.h>

#include "Fonts/FontAwesomeDescriptors.h"
#include "SpectrumUtils.h"

bool USpectrumWidgetFVector::Begin(const UWorld* World, const FVector& InVector)
{
	Vector = InVector;
	return Super::Begin(World);
}

void USpectrumWidgetFVector::SetLabel(const FString& NewLabel)
{
	Label = NewLabel;
}

void USpectrumWidgetFVector::SetEditable(bool bEditable)
{
	bIsEditable = bEditable;
}

bool USpectrumWidgetFVector::BeginInternal(const UWorld* World)
{
	bool bModified = false;

	ImGui::PushID(GetImGuiGuid());

	ImGui::TextUnformatted(TCHAR_TO_ANSI(*Label));

	// Disable editing for drag sliders
	if (!bIsEditable)
	{
		ImGui::BeginDisabled();
	}

	ImGui::SameLine();
	ImGui::DragScalarN("", ImGuiDataType_Double, &Vector.X, 3);

	// Disable editing for drag sliders
	if (!bIsEditable)
	{
		ImGui::EndDisabled();
	}

	// Copy button
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_COPY))
	{
		FPlatformApplicationMisc::ClipboardCopy(*Vector.ToString());
	}
	IMGUI_SIMPLE_TOOLTIP("Copy to clipboard");

	// Paste
	if (bIsEditable)
	{
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PASTE))
		{
			FString NewVector;
			FPlatformApplicationMisc::ClipboardPaste(NewVector);
			bModified = Vector.InitFromString(NewVector);
		}
		IMGUI_SIMPLE_TOOLTIP("Paste from clipboard");
	}

	ImGui::PopID();

	return bModified;
}
