// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Templates/SubclassOf.h>

#include "SpectrumMenu.generated.h"

class USpectrumWindow;

UCLASS()
class SPECTRUM_API USpectrumMenuEntry : public UObject
{
	GENERATED_BODY()

public:

	USpectrumMenuEntry();
	USpectrumMenuEntry(const FName& InName);

	virtual void ImGuiTick(const UWorld* World);

	FName Name = "";
	bool bClicked = false;
};

UCLASS()
class SPECTRUM_API USpectrumMenuWindowEntry : public USpectrumMenuEntry
{
	GENERATED_BODY()

public:

	// Begin USpectrumMenuEntry overrides
	virtual void ImGuiTick(const UWorld* World) override;
	// End USpectrumMenuEntry overrides

	TSubclassOf<USpectrumWindow> WindowClass;
};

UCLASS()
class SPECTRUM_API USpectrumMenu : public UObject
{
	GENERATED_BODY()

public:

	USpectrumMenu();

	virtual void Initialize();

	virtual void RegisterDebugWindowClass(TSubclassOf<USpectrumWindow> WindowClass, const FString& MenuEntryName);

	virtual void ImGuiTick(const UWorld* World);

private:

	void DrawMenuBar(const UWorld* World);
	void DrawStats(const UWorld* World);

	UPROPERTY()
	TArray<TObjectPtr<USpectrumMenuEntry>> Entries;

	// Built in listing of debugger windows
	UPROPERTY()
	TMap<TSubclassOf<USpectrumWindow>, FName> RegisteredBuiltInDebugWindowClasses;

	// Specific map to display debug windows in a specific menu
	UPROPERTY()
	TMap<TSubclassOf<USpectrumWindow>, FName> RegisteredDebugWindowClasses;
};
