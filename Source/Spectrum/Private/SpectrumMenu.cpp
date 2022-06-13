// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumMenu.h"

#include <imgui.h>

#include "SpectrumConsoleCommandWindow.h"
#include "SpectrumDebugWindowBase.h"
#include "SpectrumModule.h"
#include "SpectrumSettings.h"
#include "SpectrumSettingsWindow.h"
#include "SpectrumWorldSubsystem.h"

extern ENGINE_API float GAverageMS;

USpectrumMenuEntry::USpectrumMenuEntry()
{
}

USpectrumMenuEntry::USpectrumMenuEntry(const FName& InName)
	: Name(InName)
{
}

void USpectrumMenuEntry::ImGuiTick(const UWorld* World)
{
	bClicked = false;

	// Gather subobjects
	TArray<UObject*> SubObjects;
	ForEachObjectWithOuter(this, [&SubObjects](UObject* Object)
	{
		SubObjects.Add(Object);
	}, false);

	// Draw differently if we have subobjects
	if (SubObjects.Num() > 0)
	{
		if (ImGui::BeginMenu(TCHAR_TO_ANSI(*Name.ToString())))
		{
			for (UObject* SubObject : SubObjects)
			{
				if (USpectrumMenuEntry* MenuEntry = Cast<USpectrumMenuEntry>(SubObject))
				{
					MenuEntry->ImGuiTick(World);
				}
			}
			ImGui::EndMenu();
		}
	}
	else
	{
		if (ImGui::MenuItem(TCHAR_TO_ANSI(*Name.ToString())))
		{
			bClicked = true;
		}
	}
}

void USpectrumMenuWindowEntry::ImGuiTick(const UWorld* World)
{
	Super::ImGuiTick(World);

	if (bClicked)
	{
		USpectrumWorldSubsystem* SpectrumSubsystem = USpectrumWorldSubsystem::Get(World);
		if (IsValid(SpectrumSubsystem))
		{
			SpectrumSubsystem->NewWindow(WindowClass);
		}
	}
}

USpectrumMenu::USpectrumMenu()
{
}

void USpectrumMenu::Initialize()
{
	TArray<UClass*> DebugWindowClasses;
	GetDerivedClasses(USpectrumDebugWindowBase::StaticClass(), DebugWindowClasses, true);

	for (UClass* DebugWindowClass : DebugWindowClasses)
	{
		if (!IsValid(DebugWindowClass))
		{
			continue;
		}

		if (const USpectrumDebugWindowBase* const QuickInstance = Cast<USpectrumDebugWindowBase>(DebugWindowClass->GetDefaultObject()))
		{
			// Separate built in windows to custom ones
			if (QuickInstance->IsBuiltIn())
			{
				RegisteredBuiltInDebugWindowClasses.FindOrAdd(DebugWindowClass, QuickInstance->GetMenuTitle());
			}
			else
			{
				RegisteredDebugWindowClasses.FindOrAdd(DebugWindowClass, QuickInstance->GetMenuTitle());
			}
		}
	}
}

void USpectrumMenu::RegisterDebugWindowClass(TSubclassOf<USpectrumWindow> WindowClass, const FString& MenuEntryName)
{
	// #TODO(): [09/03/2022] Tell dev about a problem if this class isn't really valid
	if (WindowClass)
	{
		RegisteredDebugWindowClasses.FindOrAdd(WindowClass) = FName(MenuEntryName);
	}
}

void USpectrumMenu::ImGuiTick(const UWorld* World)
{
	bool bShouldDrawWhileInGame = true;

	TObjectPtr<USpectrumSettings> SpectrumSettings = FSpectrumModule::Get().GetSpectrumSettings();
	if (IsValid(SpectrumSettings))
	{
		bShouldDrawWhileInGame = SpectrumSettings->bAlwaysShowMenuBar;
	}

	// If Spectrum settings are marked to not draw the menu bar when input's focused on game then bail out immediately
	const FImGuiModuleProperties& ImGuiModuleProperties = FImGuiModule::Get().GetProperties();
	if (!ImGuiModuleProperties.IsInputEnabled() && !bShouldDrawWhileInGame)
	{
		return;
	}

	DrawMenuBar(World);
}

void USpectrumMenu::DrawMenuBar(const UWorld* World)
{
	USpectrumWorldSubsystem* SpectrumSubsystem = USpectrumWorldSubsystem::Get(World);
	if (!IsValid(SpectrumSubsystem))
	{
		return;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Settings"))
			{
				SpectrumSubsystem->NewWindow(USpectrumSettingsWindow::StaticClass());
			}

			ImGui::EndMenu();
		}

		// Render out all the registered debug window menu entries
		if (ImGui::BeginMenu("Debuggers"))
		{
			if (IsValid(SpectrumSubsystem))
			{
				for (const TPair<TSubclassOf<USpectrumWindow>, FName>& WindowClassPair : RegisteredBuiltInDebugWindowClasses)
				{
					if (ImGui::MenuItem(TCHAR_TO_ANSI(*WindowClassPair.Value.ToString())))
					{
						SpectrumSubsystem->NewWindow(WindowClassPair.Key);
					}
				}

				ImGui::Separator();

				for (const TPair<TSubclassOf<USpectrumWindow>, FName>& WindowClassPair : RegisteredDebugWindowClasses)
				{
					if (ImGui::MenuItem(TCHAR_TO_ANSI(*WindowClassPair.Value.ToString())))
					{
						SpectrumSubsystem->NewWindow(WindowClassPair.Key);
					}
				}
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Console Commands"))
			{
				SpectrumSubsystem->NewWindow(USpectrumConsoleCommandWindow::StaticClass());
			}

			ImGui::EndMenu();
		}

		for (const TObjectPtr<USpectrumMenuEntry> MenuEntry : Entries)
		{
			if (IsValid(MenuEntry))
			{
				MenuEntry->ImGuiTick(World);
			}
		}

		DrawStats(World);

		ImGui::EndMainMenuBar();
	}
}

void USpectrumMenu::DrawStats(const UWorld* World)
{
	double LastDeltaTimeMs = 0.f;

	const USpectrumWorldSubsystem* SpectrumSubsystem = USpectrumWorldSubsystem::Get(World);
	if (IsValid(SpectrumSubsystem))
	{
		LastDeltaTimeMs = SpectrumSubsystem->GetLastDeltaTimeMs();
	}

	const FImGuiModuleProperties& ImGuiModuleProperties = FImGuiModule::Get().GetProperties();

    static const char* SpectrumInputFocusStr = "Spectrum";
    static const char* GameInputFocusStr     = "Game";
    const bool ImGuiInputFocused = ImGuiModuleProperties.IsInputEnabled();

    // Draw the frame time for Spectrum and the game on the right side of the main menu
    static constexpr float Padding = 10.f;
    const float SpectrumFrametimePercentage = FMath::IsNearlyZero(GAverageMS) ? 0.f : LastDeltaTimeMs / GAverageMS * 100.f;
    const FString FrameTimeStr = FString::Printf(TEXT("%4.2f ms (%2.2f%%) | %4.2f ms | %hs [;]"), LastDeltaTimeMs, SpectrumFrametimePercentage, GAverageMS, ImGuiInputFocused ? SpectrumInputFocusStr : GameInputFocusStr);
    ImGui::SetCursorPosX(SpectrumSubsystem->GetViewportSize().X - ImGui::CalcTextSize(TCHAR_TO_ANSI(*FrameTimeStr)).x - Padding);
    ImGui::TextUnformatted(TCHAR_TO_ANSI(*FrameTimeStr));

    // "Tooltip"
    if (ImGui::IsItemHovered())
    {
    	static const char* FrameTimeTooltipStr = "Spectrum FT (Spectrum Game Contribution) | Game FT | Spectrum/Game Input Focus";

    	ImVec2 WindowPos = ImGui::GetCursorPos();
    	WindowPos.x += -ImGui::CalcTextSize(FrameTimeTooltipStr).x - 15.f;
    	WindowPos.y += 20.f;
    	ImGui::SetNextWindowPos(WindowPos);

    	const ImGuiWindowFlags Flags = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
    	ImGui::Begin("##SpectrumInfoTooltip", nullptr, Flags);
    	ImGui::TextUnformatted(FrameTimeTooltipStr);
    	ImGui::EndTooltip();
    }
}
