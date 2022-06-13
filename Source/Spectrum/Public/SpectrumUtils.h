#pragma once

#include <imgui.h>
#include <Math/Color.h>

static ImVec4 UEColorToImVec4(const FLinearColor& LinearColor)
{
	return ImVec4(LinearColor.R, LinearColor.G, LinearColor.B, LinearColor.A);
}

static ImVec4 UEColorToImVec4(const FColor& Color)
{
	return UEColorToImVec4(Color.ReinterpretAsLinear());
}

// Helpful for converting FontAwesome glyphs to FString. Evaluated at runtime.
#define UTF8_FSTRING(String) FString(UTF8_TO_TCHAR(String))
#define ICON(String) UTF8_FSTRING(String)

// Creates a very simple textual tooltip for current ImGui item
#define IMGUI_SIMPLE_TOOLTIP(CharPtr) \
	if (ImGui::IsItemHovered())       \
	{                                 \
		ImGui::SetTooltip(CharPtr);   \
	}
