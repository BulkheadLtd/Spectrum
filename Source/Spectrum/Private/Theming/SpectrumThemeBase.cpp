// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Theming/SpectrumThemeBase.h"

#include <imgui.h>
#include <imgui_internal.h>

FName USpectrumThemeBase::GetThemeName() const
{
	return NAME_None;
}

void USpectrumThemeBase::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}
    
	Context->Style = ImGuiStyle();
}
