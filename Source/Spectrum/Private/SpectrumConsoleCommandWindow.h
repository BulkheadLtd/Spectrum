// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWindow.h"

#include "SpectrumConsoleCommandWindow.generated.h"

class IConsoleObject;

UCLASS()
class SPECTRUM_API USpectrumConsoleCommandWindow : public USpectrumWindow
{
	GENERATED_BODY()

public:

	USpectrumConsoleCommandWindow();

	// Begin USpectrumWindow overrides
	virtual void DrawWindowContent(float DeltaTime, UWorld* World) override;
	virtual FString GetTitle() override;
	virtual FVector2D GetInitialSize() override;
	// End USpectrumWindow overrides

private:

	void ConsoleObjectVisitor(const TCHAR* Name, IConsoleObject* ConsoleObject);

	TMap<FName, TMap<FName, IConsoleObject*>> ConsoleCommands;

	// Not exactly the best approach because for some reason UE4's console commands aren't very well organized...
	// #TODO(): [13/03/2022] Different approach to make filtering more flexible.
	TMap<FName, FName> FilterMap = {
		{"r",                "Rendering"},
		{"Slate",            "Slate"},
		{"SlateDebugger",    "Slate"},
		{"WidgetComponent",  "Slate"},
		{"Widget",           "Slate"},
		{"a",                "Animation"},
		{"p",                "Physics"},
		{"au",               "Audio"},
		{"sg",               "Shader Graph"},
		{"net",              "Networking"},
		{"fx",               "FX"},
		{"ShowFlag",         "ShowFlag"},
		{"VREd",             "VR/AR"},
		{"vr",               "VR/AR"},
		{"ar",               "VR/AR"},
		{"ai",               "AI"},
	};

	FName SelectedFilter = NAME_None;
};
