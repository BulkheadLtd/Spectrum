// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Theming/SpectrumBuiltInThemes.h"

#include <imgui.h>
#include <imgui_internal.h>

ImVec4 FColorToImVec4(const FColor& InColor)
{
	ImVec4 Result;

	FLinearColor LinearColor = InColor;
	Result.x = LinearColor.R;
	Result.y = LinearColor.G;
	Result.z = LinearColor.B;
	Result.w = LinearColor.A;
	
	return Result;
}

FName USpectrumThemeDefault::GetThemeName() const
{
	return "Programmer Art";
}

void USpectrumThemeDefault::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}

	Context->Style = ImGuiStyle();
}

FName USpectrumThemeConterStronk16::GetThemeName() const
{
	return "ConterStronk 16";
}

void USpectrumThemeConterStronk16::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}

	Super::ApplyTheme(Context);

	// Theme taken from: https://github.com/ocornut/imgui/issues/707#issuecomment-576867100
	
	ImGuiStyle& Style = Context->Style;
	ImVec4* Colors    = Style.Colors;
	
    Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    Colors[ImGuiCol_WindowBg]              = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    Colors[ImGuiCol_ChildBg]               = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    Colors[ImGuiCol_PopupBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    Colors[ImGuiCol_Border]                = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    Colors[ImGuiCol_BorderShadow]          = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
    Colors[ImGuiCol_FrameBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
    Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
    Colors[ImGuiCol_TitleBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
    Colors[ImGuiCol_CheckMark]             = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_SliderGrab]            = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    Colors[ImGuiCol_Button]                = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
    Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    Colors[ImGuiCol_ButtonActive]          = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    Colors[ImGuiCol_Header]                = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.35f, 0.42f, 0.31f, 0.60f);
    Colors[ImGuiCol_HeaderActive]          = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
    Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.19f, 0.23f, 0.18f, 0.00f);
    Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_Tab]                   = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    Colors[ImGuiCol_TabHovered]            = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
    Colors[ImGuiCol_TabActive]             = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    //colors[ImGuiCol_DockingPreview]      = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    //colors[ImGuiCol_DockingEmptyBg]      = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
    Colors[ImGuiCol_NavHighlight]          = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    Style.FrameBorderSize   = 1.0f;
    Style.WindowRounding    = 0.0f;
    Style.ChildRounding     = 0.0f;
    Style.FrameRounding     = 0.0f;
    Style.PopupRounding     = 0.0f;
    Style.ScrollbarRounding = 0.0f;
    Style.GrabRounding      = 0.0f;
    Style.TabRounding       = 0.0f;
}

FName USpectrumThemePeaceCats::GetThemeName() const
{
	return "PeaceCats";
}

void USpectrumThemePeaceCats::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}

	Super::ApplyTheme(Context);

	// Theme taken from: https://github.com/ocornut/imgui/issues/707#issuecomment-622934113
	
	ImGuiStyle& Style = Context->Style;
	ImVec4* Colors    = Style.Colors;

    Colors[ImGuiCol_Text]                   = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    Colors[ImGuiCol_TextDisabled]           = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    Colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    Colors[ImGuiCol_Border]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_FrameBg]                = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_TitleBg]                = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    Colors[ImGuiCol_CheckMark]              = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_SliderGrab]             = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_Button]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_ButtonActive]           = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_Header]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_HeaderActive]           = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
    Colors[ImGuiCol_Separator]              = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_Tab]                    = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    Colors[ImGuiCol_TabHovered]             = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    Colors[ImGuiCol_TabActive]              = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    Colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    Colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    Colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    Style.FramePadding = ImVec2(4, 2);
    Style.ItemSpacing = ImVec2(10, 2);
    Style.IndentSpacing = 12;
    Style.ScrollbarSize = 10;
    Style.WindowRounding = 4;
    Style.FrameRounding = 4;
    Style.PopupRounding = 4;
    Style.ScrollbarRounding = 6;
    Style.GrabRounding = 4;
    Style.TabRounding = 4;
    Style.WindowTitleAlign = ImVec2(1.0f, 0.5f);
    Style.WindowMenuButtonPosition = ImGuiDir_Right;
    Style.DisplaySafeAreaPadding = ImVec2(4, 4);
}

FName USpectrumThemeGrim::GetThemeName() const
{
	return "Grim";
}

void USpectrumThemeGrim::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}

	Super::ApplyTheme(Context);

	// Theme taken from: https://github.com/ocornut/imgui/issues/707#issuecomment-678611331
	
	ImGuiStyle& Style = Context->Style;
	ImVec4* Colors    = Style.Colors;
	
	Style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	Style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	Style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	Style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	Style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	Style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	Style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	Style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	Style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	Style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	Style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	Style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	Style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	Style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	Style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	Style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	Style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	Style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	Style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	Style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	Style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	Style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	Style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	Style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	Style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	Style.Colors[ImGuiCol_Separator]             = Style.Colors[ImGuiCol_Border];
	Style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	Style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	Style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	Style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	Style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	Style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	Style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	Style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	Style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	// style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	// style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	Style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	Style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	Style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	Style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	Style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	Style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	Style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	Style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	Style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	Style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	Style.GrabRounding                           = Style.FrameRounding = 2.3f;
}

FName USpectrumThemeDeepDark::GetThemeName() const
{
	return "Deep Dark";
}

void USpectrumThemeDeepDark::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
		return;
	}

	Super::ApplyTheme(Context);

	// Theme taken from: https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
	
	ImGuiStyle& Style = Context->Style;
	ImVec4* Colors    = Style.Colors;

	Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	Colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	Colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	Colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	Colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	Colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	Colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	// Colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	// Colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	Style.WindowPadding                     = ImVec2(8.00f, 8.00f);
	Style.FramePadding                      = ImVec2(5.00f, 2.00f);
	Style.CellPadding                       = ImVec2(6.00f, 6.00f);
	Style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
	Style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
	Style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
	Style.IndentSpacing                     = 25;
	Style.ScrollbarSize                     = 15;
	Style.GrabMinSize                       = 10;
	Style.WindowBorderSize                  = 1;
	Style.ChildBorderSize                   = 1;
	Style.PopupBorderSize                   = 1;
	Style.FrameBorderSize                   = 1;
	Style.TabBorderSize                     = 1;
	Style.WindowRounding                    = 7;
	Style.ChildRounding                     = 4;
	Style.FrameRounding                     = 3;
	Style.PopupRounding                     = 4;
	Style.ScrollbarRounding                 = 9;
	Style.GrabRounding                      = 3;
	Style.LogSliderDeadzone                 = 4;
	Style.TabRounding                       = 4;
}

FName USpectrumThemePixi::GetThemeName() const
{
	return "Pixi";
}

void USpectrumThemePixi::ApplyTheme(ImGuiContext* Context)
{
	if (Context == nullptr)
	{
	    return;
	}

	Super::ApplyTheme(Context);

	// Theme taken from: https://github.com/foxnne/pixi/blob/main/src/editor/editor.zig#L52

	ImGuiStyle& Style = Context->Style;
	ImVec4* Colors    = Style.Colors;

	const ImVec4 background_color = FColorToImVec4(FColor(30, 31, 39, 255));
	const ImVec4 foreground_color = FColorToImVec4(FColor(42, 44, 54, 255));
	const ImVec4 text_color       = FColorToImVec4(FColor(230, 175, 137, 255));

	const ImVec4 highlight_color_green       = FColorToImVec4(FColor(47, 179, 135, 255));
	const ImVec4 highlight_hover_color_green = FColorToImVec4(FColor(76, 148, 123, 255));

	const ImVec4 highlight_color_red       = FColorToImVec4(FColor(76, 48, 67, 255));
	const ImVec4 highlight_hover_color_red = FColorToImVec4(FColor(105, 50, 68, 255));

	Colors[ImGuiCol_WindowBg] = background_color;
	Colors[ImGuiCol_Border] = foreground_color;
	Colors[ImGuiCol_MenuBarBg] = foreground_color;
	//Colors[ImGuiCol_DockingEmptyBg] = background_color;
	Colors[ImGuiCol_Separator] = foreground_color;
	Colors[ImGuiCol_TitleBg] = background_color;
	Colors[ImGuiCol_Tab] = background_color;
	Colors[ImGuiCol_TabUnfocused] = background_color;
	Colors[ImGuiCol_TabUnfocusedActive] = background_color;
	Colors[ImGuiCol_TitleBgActive] = foreground_color;
	Colors[ImGuiCol_TabActive] = foreground_color;
	Colors[ImGuiCol_TabHovered] = foreground_color;
	Colors[ImGuiCol_PopupBg] = background_color;
	Colors[ImGuiCol_Text] = text_color;
	Colors[ImGuiCol_ResizeGrip] = highlight_color_green;
	Colors[ImGuiCol_ScrollbarGrabActive] = highlight_color_green;
	Colors[ImGuiCol_ScrollbarGrabHovered] = highlight_hover_color_green;

	Colors[ImGuiCol_Header] = highlight_color_red;
	Colors[ImGuiCol_HeaderHovered] = highlight_hover_color_red;
	Colors[ImGuiCol_HeaderActive] = highlight_color_red;
	Colors[ImGuiCol_ScrollbarBg] = background_color;
	Colors[ImGuiCol_ScrollbarGrab] = foreground_color;
	//Colors[ImGuiCol_DockingPreview] = highlight_color_green;
	Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(10, 10, 15, 100);

	Style.TabRounding = 2;
	Style.FrameRounding = 8;
	Style.WindowBorderSize = 1;
	Style.WindowRounding = 0;
	Style.WindowMinSize = ImVec2(100, 100);
	Style.WindowMenuButtonPosition = ImGuiDir_None;
	Style.PopupRounding = 8;
	Style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
}
