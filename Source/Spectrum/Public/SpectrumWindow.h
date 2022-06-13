// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWindow.generated.h"

DECLARE_DELEGATE(FSpectrumWindowDrawDelegate);

// UE string to ImGui 
#define RAWCHARPTR(S) TCHAR_TO_ANSI(*S)

UCLASS(Abstract)
class SPECTRUM_API USpectrumWindow : public UObject
{
	GENERATED_BODY()

public:

	USpectrumWindow();
	USpectrumWindow(const FString& InTitle, const FString& InMainMenuTitle, const FString& InSubmenuEntryTitle);
	virtual ~USpectrumWindow() override;
	
	virtual FString GetTitle();
	virtual FVector2D GetInitialSize();

	// Override to create custom internal behaviour.
	// WARNING: Don't override this unless you know what you're doing. Rather @see DrawWindowContent if you just want to
	//          do some general drawing in a window.
	virtual void ImGuiTick(float DeltaTime, UWorld* World);

	// Override this if you just want to draw in the window
	virtual void DrawWindowContent(float DeltaTime, UWorld* World);

	int32 GetImGuiGuid() const;

	void SetIsVisible(bool bSetVisible);
	bool GetIsVisible();
	bool* GetIsVisiblePtr();
	bool* GetIsCollapsedPtr();

protected:

	void BeginRespectWindowTransparencySetting();
	void EndRespectWindowTransparencySetting();

	// The ID passed to ImGui for identifying this window. Handy for checking whether this window already exists or not
	FGuid Guid = FGuid::NewGuid();

	// Actual window's title
	FString Title = "";

	// Function called when drawing. Mostly for when devs don't want to inherit from this class and just want
	// Spectrum to call into their own class's method for drawing so they have access to the relevant information.
	FSpectrumWindowDrawDelegate OnImGuiTick;
	FSpectrumWindowDrawDelegate OnDrawWindowContent;

	// Force self destruction when the X button is pressed on the window
	bool bDestroyOnClose = true;

	bool bIsCollapsed = false;

	// Spawn as visible
	bool bIsVisible = true;
};
