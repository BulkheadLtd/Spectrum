// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <Subsystems/WorldSubsystem.h>

#include <ImGuiModule.h>

#include "SpectrumWorldSubsystem.generated.h"

class USpectrumMenu;
class USpectrumWindow;

UCLASS()
class SPECTRUM_API USpectrumWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// Begin USubsystem overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem overrides

	// Begin UWorldSubsystem overrides
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	// End UWorldSubsystem overrides

	virtual ~USpectrumWorldSubsystem() override;

	// Convenience method
	static USpectrumWorldSubsystem* Get(const UWorld* World);

	// Allows executing a functor for every world subsystem that's initialized in the game
	static void ExecForEach(TFunctionRef<void(USpectrumWorldSubsystem*)> Functor);

	void FirstTimeConfig();

	void OnImGuiWorldTick();

	// Creates new instances of a window in the viewport
	void NewWindow(TSubclassOf<USpectrumWindow> WindowClass);

	// Destroys a specific window from the viewport
	void DestroyWindow(TObjectPtr<USpectrumWindow> WindowToDestroy);

	FVector2D GetViewportSize() const;
	double GetLastDeltaTimeMs() const;

	void RequestThemeReload();

private:

	FImGuiDelegateHandle ImGuiWorldDelegate;

	bool bIsConfigured = false;

	// Last delta time the local tick ran for
	double LastDeltaTimeMS = 0.f;

	// Last timestamp the game thread ticked over. Helpful for tracking final frame time.
	double LastGameThreadTime = 0.f;

	// Window instances in memory. Usually these are rendering this frame
	UPROPERTY()
	TArray<TObjectPtr<USpectrumWindow>> WindowInstances;

	// Windows that will be removed from view the end of this frame. UE garbage collector then cleans them up
	UPROPERTY()
	TArray<TObjectPtr<USpectrumWindow>> PendingWindowRemovals;

	// Top menu
	UPROPERTY()
	TObjectPtr<USpectrumMenu> Menu;

	bool bThemeHasChanged = false;
};
