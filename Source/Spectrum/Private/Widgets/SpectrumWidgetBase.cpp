// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetBase.h"

#include <imgui.h>

void USpectrumWidgetBase::SetCustomSize(FVector2D NewSize)
{
	CustomSize = NewSize;
}

bool USpectrumWidgetBase::Begin(const UWorld* World)
{
	// We need to push an ID onto the ImGui stack so that this widget remains unique
	ImGui::PushID(GetImGuiGuid());
	return BeginInternal(World);
}

void USpectrumWidgetBase::End()
{
	EndInternal();
	ImGui::PopID();
}

int32 USpectrumWidgetBase::GetImGuiGuid() const
{
	return Guid.A + Guid.B + Guid.C + Guid.D;
}

bool USpectrumWidgetBase::BeginInternal(const UWorld* World)
{
	return false;
}
