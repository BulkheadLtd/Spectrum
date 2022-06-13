// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include <Camera/CameraTypes.h>
#include <Misc/EnumRange.h>

#include "SpectrumDebugWindowBase.h"

#include "SpectrumDebugWindowVTCamera.generated.h"

class USpectrumWidgetTextureRT;
class ASceneCapture2D;

UENUM()
enum class ESpectrumRViewCameraType : uint8
{
	Attached       UMETA(DisplayName = "Attached"),
	FreeCam        UMETA(DisplayName = "Free Cam"),
	
	Count          UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESpectrumRViewCameraType, ESpectrumRViewCameraType::Count);

UCLASS()
class SPECTRUM_API USpectrumDebugWindowVTCamera : public USpectrumDebugWindowBase
{
	GENERATED_BODY()

public:

	USpectrumDebugWindowVTCamera();
	~USpectrumDebugWindowVTCamera();
	
	// Begin USpectrumWindow overrides
	virtual FString GetTitle() override;
	virtual FVector2D GetInitialSize() override;
	virtual void DrawWindowContent(float DeltaTime, UWorld* World) override;
	virtual bool IsBuiltIn() const override;
	// End USpectrumWindow overrides

	// Begin USpectrumDebugWindowBase overrides
	virtual FName GetMenuTitle() const override;
	// End USpectrumDebugWindowBase overrides

	void SetCameraToLocalPlayersEye(const UWorld* World) const;

private:

	void RecalculateCameraProjectionMatrix(const FVector2D& ViewportSize);
	void ControlledCameraMove(float DeltaTime, UWorld* World, const FVector2D& MouseDelta);
	void DrawCameraControls(UWorld* World);
	void DrawCameraControlsInViewport(UWorld* World, const FVector2D& ViewportPos, const FVector2D& ViewportSize);
	FString CameraTypeToString(ESpectrumRViewCameraType InCameraType) const;
	void OnCameraTypeChanged(UWorld* World);

	UPROPERTY()
	TObjectPtr<USpectrumWidgetTextureRT> WidgetTextureRT = nullptr;

	UPROPERTY()
	TObjectPtr<ASceneCapture2D> SceneCapture2D = nullptr;

	bool bIsInitialized = false;

	float DesiredCameraFOV = 90.f;

	ESpectrumRViewCameraType CameraType = ESpectrumRViewCameraType::FreeCam;

	bool bIsControllingCamera = false;

	float ScrollScalar = 3.f;

	float MouseSensitivity = 20.f;

	float CameraSpeed = 250.f;

	// If true, makes camera move along 2D plane and requires jump/crouch to go up or down. If false then camera
	// acts like UE4's editor camera.
	bool bPlanarCameraMovement = false;

	bool bSquareViewport = false;

	FMinimalViewInfo CustomViewInfo;

	bool bDrawCameraInWorld = false;

	FVector2D LockInPoint = FVector2D::ZeroVector;
};
