// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumWorldSubsystem.h"

#include <Engine/Engine.h>
#include <Engine/World.h>
#include <Engine/GameViewportClient.h>
#include <GameFramework/PlayerController.h>

#include <implot.h>

#include "SpectrumMenu.h"
#include "SpectrumModule.h"
#include "SpectrumSettings.h"
#include "SpectrumWindow.h"
#include "Theming/SpectrumThemeManager.h"

// FontAwesome
#include "Fonts/FontAwesomeDescriptors.h"
#include "Fonts/FontAwesomeFont.h"

void USpectrumWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void USpectrumWorldSubsystem::Deinitialize()
{
	FImGuiModule::Get().RemoveImGuiDelegate(ImGuiWorldDelegate);
}

void USpectrumWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.WorldType == EWorldType::PIE || InWorld.WorldType == EWorldType::Game)
	{
		Menu = NewObject<USpectrumMenu>(this);
		if (IsValid(Menu))
		{
			Menu->Initialize();
		}

		ImGuiWorldDelegate = FImGuiModule::Get().AddWorldImGuiDelegate(&InWorld, FImGuiDelegate::CreateUObject(this, &USpectrumWorldSubsystem::OnImGuiWorldTick));

		// Tell ImGui settings the world has begun
		TObjectPtr<USpectrumSettings> Settings = FSpectrumModule::Get().GetSpectrumSettings();
		if (IsValid(Settings))
		{
			Settings->OnWorldBegin(&InWorld);
		}
	}
}

USpectrumWorldSubsystem::~USpectrumWorldSubsystem()
{
	FImGuiModule::Get().RemoveImGuiDelegate(ImGuiWorldDelegate);
}

USpectrumWorldSubsystem* USpectrumWorldSubsystem::Get(const UWorld* World)
{
	USpectrumWorldSubsystem* Result = nullptr;
	if (IsValid(World))
	{
		Result = World->GetSubsystem<USpectrumWorldSubsystem>();
	}
	return Result;
}

void USpectrumWorldSubsystem::ExecForEach(TFunctionRef<void(USpectrumWorldSubsystem*)> Functor)
{
	if (!IsValid(GEngine))
	{
		return;
	}

	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (const UWorld* World = WorldContext.World())
		{
			USpectrumWorldSubsystem* SpectrumWorldSubsystem = World->GetSubsystem<USpectrumWorldSubsystem>();
			if (IsValid(SpectrumWorldSubsystem))
			{
				Functor(SpectrumWorldSubsystem);
			}
		}
	}
}

void USpectrumWorldSubsystem::FirstTimeConfig()
{
	ImGuiIO& IO = ImGui::GetIO();

	// Add FontAwesome font glyphs from memory
	if (TSharedPtr<ImFontConfig> FAFontConfig = MakeShareable(new ImFontConfig()))
	{
		static const ImWchar IconRange[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		FAFontConfig->FontDataOwnedByAtlas = false;
		FAFontConfig->FontData = (void*)FontAwesomeData;
		FAFontConfig->FontDataSize = FontAwesomeSize;
		FAFontConfig->SizePixels = 16;
		FAFontConfig->MergeMode = true;
		FAFontConfig->GlyphRanges = IconRange;
		FAFontConfig->GlyphMinAdvanceX = 15.f;
		FAFontConfig->PixelSnapH = true;
		FAFontConfig->GlyphOffset = { 0, 3 };

		FImGuiModule::Get().GetProperties().AddCustomFont("FontAwesome", FAFontConfig);
		FImGuiModule::Get().RebuildFontAtlas();
	}

	// Allow windows to be resized from edges
	IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	IO.ConfigWindowsResizeFromEdges = true;

	// Enable experimental docking support
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Apply saved theme
	const TObjectPtr<USpectrumThemeManager> ThemeManager = FSpectrumModule::Get().GetThemeManager();
	if (IsValid(ThemeManager))
	{
		ThemeManager->ApplyCurrentTheme(ImGui::GetCurrentContext());
	}

	bIsConfigured = true;
}

void USpectrumWorldSubsystem::OnImGuiWorldTick()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(USpectrumWorldSubsystem::OnImGuiWorldTick);

	float GameThreadDiffTime;
	if (FApp::IsBenchmarking() || FApp::UseFixedTimeStep())
	{
		// If we're in fixed time step mode, FApp::GetCurrentTime() will be incorrect for benchmarking
		const double GameThreadCurrentTime = FPlatformTime::Seconds();
		if (LastGameThreadTime == 0)
		{
			LastGameThreadTime = GameThreadCurrentTime;
		}
		GameThreadDiffTime = GameThreadCurrentTime - LastGameThreadTime;
		LastGameThreadTime = GameThreadCurrentTime;
	}
	else
	{
		// Use the DiffTime we computed last frame, because it correctly handles the end of frame idling and corresponds better to the other unit times
		GameThreadDiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	}

	if (!bIsConfigured)
	{
		FirstTimeConfig();
	}

	UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	ImPlot::GetStyle().AntiAliasedLines = true;

	FImGuiModuleProperties& ImGuiModuleProperties = FImGuiModule::Get().GetProperties();
	const bool bIsImGuiInputEnabled = ImGuiModuleProperties.IsInputEnabled();

	// #TODO(): [26/02/2022] Probably a better way of getting the player's controller in this world
	// without having to try every tick but this will do for now.
	// Get the local player controller for this world/tick
	const APlayerController* PlayerController = World->GetFirstPlayerController();

	if (ImGui::IsKeyPressed(ImGuiKey_Semicolon, false))
	{
		ImGuiModuleProperties.SetInputEnabled(false);
	}

	if (IsValid(PlayerController))
	{
		if (PlayerController->WasInputKeyJustPressed(EKeys::Semicolon))
		{
			ImGuiModuleProperties.SetInputEnabled(true);
		}
	}

	const double StartSpectrumTick = FPlatformTime::Seconds();

	// Reset viewport size to zero in case we don't have a local player in world
	FVector2D ViewportSize = GetViewportSize();

	// Render menu bar
	if (IsValid(Menu))
	{
		Menu->ImGuiTick(World);
	}

	// Finally, draw any windows
	for (const TObjectPtr<USpectrumWindow> Window : WindowInstances)
	{
		if (!IsValid(Window))
		{
			continue;
		}

		Window->ImGuiTick(GameThreadDiffTime, World);
	}

	// Remove any pending kill requests from the window instances array. UE will deal with cleanup of the object
	// when the garbage collector kicks in.
	for (TObjectPtr<USpectrumWindow> Window : PendingWindowRemovals)
	{
		WindowInstances.Remove(Window);
	}
	PendingWindowRemovals.Reset();

	// Apply selected theme at the end of the frame
	if (bThemeHasChanged)
	{
		bThemeHasChanged = false;

		const TObjectPtr<USpectrumThemeManager> ThemeManager = FSpectrumModule::Get().GetThemeManager();
		if (IsValid(ThemeManager))
		{
			ThemeManager->ApplyCurrentTheme(ImGui::GetCurrentContext());
		}
	}

	const double EndSpectrumTick = FPlatformTime::Seconds();
	LastDeltaTimeMS = (EndSpectrumTick - StartSpectrumTick) * 1000.f;
}

void USpectrumWorldSubsystem::NewWindow(TSubclassOf<USpectrumWindow> WindowClass)
{
	WindowInstances.Add(NewObject<USpectrumWindow>(this, WindowClass));
}

void USpectrumWorldSubsystem::DestroyWindow(TObjectPtr<USpectrumWindow> WindowToDestroy)
{
	PendingWindowRemovals.Add(WindowToDestroy);
}

FVector2D USpectrumWorldSubsystem::GetViewportSize() const
{
	FVector2D Result = FVector2D::ZeroVector;

	if (const UWorld* World = GetWorld())
	{
		// Fetch the viewport size to be available throughout the manager
		const UGameViewportClient* GameViewport = World->GetGameViewport();
		if (IsValid(GameViewport))
		{
			GameViewport->GetViewportSize(Result);
		}
	}

	return Result;
}

double USpectrumWorldSubsystem::GetLastDeltaTimeMs() const
{
	return LastDeltaTimeMS;
}

void USpectrumWorldSubsystem::RequestThemeReload()
{
	bThemeHasChanged = true;
}
