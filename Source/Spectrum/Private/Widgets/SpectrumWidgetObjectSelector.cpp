// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetObjectSelector.h"

#include <imgui.h>

bool USpectrumWidgetObjectSelector::BeginInternal(const UWorld* World)
{
	bool bHasSelectionChanged = false;
	
	const FString PreviewLabel = GetObjectLabel(GetSelectedObject());

	ImGui::SetNextItemWidth(-FLT_MIN);
	if (ImGui::BeginCombo("", TCHAR_TO_ANSI(*PreviewLabel), 0))
	{
		for (int32 ObjectIdx = 0; ObjectIdx < AvailableObjects.Num(); ++ObjectIdx)
		{
			ImGui::PushID(ObjectIdx);

			const FString Label = GetObjectLabel(AvailableObjects[ObjectIdx]);
			const bool bIsSelected = (SelectedObjectIdx == ObjectIdx);

			if (ImGui::Selectable(TCHAR_TO_ANSI(*Label), bIsSelected))
			{
				SetSelectedObjectIndex(ObjectIdx);
				bHasSelectionChanged = true;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	return bHasSelectionChanged;
}

void USpectrumWidgetObjectSelector::SetAvailableObjects(const TArray<TObjectPtr<UObject>>& InObjects)
{
	const TObjectPtr<UObject> PrevSelectedObject = GetSelectedObject();

	AvailableObjects = InObjects;

	// Try to select the same object in the new array
	SetSelectedObject(PrevSelectedObject);
}

void USpectrumWidgetObjectSelector::SetSelectedObject(const TObjectPtr<UObject> InObject)
{
	for (int32 ObjectIdx = 0; ObjectIdx < AvailableObjects.Num(); ++ObjectIdx)
	{
		if (AvailableObjects[ObjectIdx] == InObject)
		{
			SetSelectedObjectIndex(ObjectIdx);
			break;
		}
	}
}

void USpectrumWidgetObjectSelector::SetSelectedObjectIndex(const int32 InObjectIdx)
{
	if (AvailableObjects.IsValidIndex(InObjectIdx))
	{
		SelectedObjectIdx = InObjectIdx;
	}
	else
	{
		SelectedObjectIdx = INDEX_NONE;
	}
}

TArray<TObjectPtr<UObject>> USpectrumWidgetObjectSelector::GetAvailableObjects() const
{
	return AvailableObjects;
}

TObjectPtr<UObject> USpectrumWidgetObjectSelector::GetSelectedObject() const
{
	TObjectPtr<UObject> Result = nullptr;

	if (AvailableObjects.IsValidIndex(SelectedObjectIdx))
	{
		Result = AvailableObjects[SelectedObjectIdx];
	}

	return Result;
}

int32 USpectrumWidgetObjectSelector::GetSelectedObjectIndex() const
{
	return SelectedObjectIdx;
}

FString USpectrumWidgetObjectSelector::GetObjectLabel(const TObjectPtr<UObject> Object) const
{
	FString Result;

	if (GetObjectLabelOverride.IsBound())
	{
		Result = GetObjectLabelOverride.Execute(Object);
	}
	else
	{
		Result = GetNameSafe(Object);
	}

	return Result;
}
