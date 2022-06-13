// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumDebugWindowVTCamera.h"

#include <EngineUtils.h>
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/SceneCapture2D.h>
#include <GameFramework/PlayerController.h>
#include <DrawDebugHelpers.h>
#include <Framework/Application/SlateApplication.h>

#include "ImGuiModule.h"

#include "Widgets/SpectrumWidgetTextureRT.h"
#include "Fonts/FontAwesomeDescriptors.h"

#include "SpectrumUtils.h"

USpectrumDebugWindowVTCamera::USpectrumDebugWindowVTCamera()
{
	// Create texture of nominal and static size (for now) of 1024 so we can still maintain a decent image quality when
	// scaling up but doesn't absolutely kill FPS when in use.
	WidgetTextureRT = CreateDefaultSubobject<USpectrumWidgetTextureRT>("TextureRT");
	if (IsValid(WidgetTextureRT))
	{
		WidgetTextureRT->Initialize({1024, 1024});
	}
}

USpectrumDebugWindowVTCamera::~USpectrumDebugWindowVTCamera()
{
	// Allow GC to come clean this actor up once we close the window
	if (IsValid(SceneCapture2D))
	{
		SceneCapture2D->Destroy();
	}
}

FString USpectrumDebugWindowVTCamera::GetTitle()
{
	return "Virtual Camera";
}

FVector2D USpectrumDebugWindowVTCamera::GetInitialSize()
{
	return {500, 600};
}

void USpectrumDebugWindowVTCamera::DrawWindowContent(float DeltaTime, UWorld* World)
{
	if (!IsValid(World))
	{
		return;
	}

	// Initialize the window on the first frame (we don't have a way at time of writing to explicitly initialize a window
	// on creation with a World context).
	if (!bIsInitialized)
	{
		// Spawn a scene capture actor into the world so we can use that to render to an offscreen FBO
		FActorSpawnParameters SpawnInfo;
		SceneCapture2D = World->SpawnActor<ASceneCapture2D>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		if (IsValid(SceneCapture2D))
		{
			USceneCaptureComponent2D* const SceneCaptureComponent2D = SceneCapture2D->GetCaptureComponent2D();
			if (IsValid(SceneCaptureComponent2D))
			{
				SceneCaptureComponent2D->TextureTarget = WidgetTextureRT->GetTextureRenderTarget();
				SceneCaptureComponent2D->UpdateContent();

				// Need to use this in order to force capture to use tone curve and also not use alpha channel
				SceneCaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalToneCurveHDR;
			}
		}

		// Set camera at local player's eyes upon initialization
		SetCameraToLocalPlayersEye(World);

		bIsInitialized = true;

		return;
	}

	DrawCameraControls(World);

	// Draw the render target texture
	if (IsValid(WidgetTextureRT))
	{
		// Try fill the rest of the window
		FVector2D ViewportSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};

		// Keep aspect ratio (1:1) if enabled
		if (bSquareViewport)
		{
			FVector2D NewTextureRTSize = ViewportSize;
			if (ViewportSize.X < ViewportSize.Y)
			{
				ViewportSize.Y = NewTextureRTSize.X;
			}
			else
			{
				ViewportSize.X = NewTextureRTSize.Y;
			}
		}

		// Get the camera to recalculate its projection matrix so we don't distort the rendered FBO
		RecalculateCameraProjectionMatrix(ViewportSize);

		// Cache the current cursor so we can draw over the viewport later
		ImVec2 CursorPos = ImGui::GetCursorPos();
		
		WidgetTextureRT->SetCustomSize(ViewportSize);
		WidgetTextureRT->Begin(World);

		// Routines for camera control
		// #TODO(): [28/03/2022] Neaten this up a bit. No need for this to be inline
		const bool bWasControllingCamera = bIsControllingCamera;

		// Not controlling camera any more if releasing button
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			bIsControllingCamera = false;
			//FImGuiModule::Get().GetProperties().SetCursorHidden(false);
		}

		// If right click pressed inside the viewport then initiate movement
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				bIsControllingCamera = true;

				// Set lock point and then hide mouse
				LockInPoint = FSlateApplication::Get().GetCursorPos();
			}

			if (!bIsControllingCamera)
			{
				ImGuiIO& IO = ImGui::GetIO();
				
				// Allow FoV change via scroll wheel while not controlling but hovering
				DesiredCameraFOV -= IO.MouseWheel * ScrollScalar;
				DesiredCameraFOV = FMath::Clamp(DesiredCameraFOV, 0.1f, 179.99f);
			}
		}
		
		if (bIsControllingCamera)
		{
			ControlledCameraMove(DeltaTime, World, FSlateApplication::Get().GetCursorPos() - LockInPoint);

			// Keep OS cursor at our lock in point
			FSlateApplication::Get().SetCursorPos(LockInPoint);
		}

		DrawCameraControlsInViewport(World, {CursorPos.x, CursorPos.y}, ViewportSize);
	}

	if (IsValid(SceneCapture2D))
	{
		if (bDrawCameraInWorld)
		{
			FVector FinalDrawPosition = SceneCapture2D->GetActorLocation();
			// Push the debug draw behind the frustum of the scene capture so we don't see it in view
			FinalDrawPosition -= SceneCapture2D->GetActorForwardVector() * 10.f; 
			DrawDebugCamera(World, FinalDrawPosition, SceneCapture2D->GetActorRotation(), DesiredCameraFOV, 1, FColor::Cyan);
		}	
	}
}

bool USpectrumDebugWindowVTCamera::IsBuiltIn() const
{
	return true;
}

FName USpectrumDebugWindowVTCamera::GetMenuTitle() const
{
	return "Virtual Camera";
}

void USpectrumDebugWindowVTCamera::SetCameraToLocalPlayersEye(const UWorld* World) const
{
	if (!IsValid(SceneCapture2D) || !IsValid(World))
	{
		return;
	}
	
	const APlayerController* const PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}
	
	FVector Position;
	FRotator Rotation;
	PlayerController->GetActorEyesViewPoint(Position, Rotation);
			
	SceneCapture2D->SetActorLocation(Position);
	SceneCapture2D->SetActorRotation(Rotation);
}

void USpectrumDebugWindowVTCamera::RecalculateCameraProjectionMatrix(const FVector2D& ViewportSize)
{
	if (!IsValid(SceneCapture2D))
	{
		return;
	}

	USceneCaptureComponent2D* const SceneCaptureComponent = SceneCapture2D->GetCaptureComponent2D();
	if (!IsValid(SceneCaptureComponent))
	{
		return;
	}

	// #TODO(): [28/03/2022] Maybe don't do this every frame and only do it when viewport size changes or when FoV changes?

	// Use the FoV and the requested viewport size to determine the projection matrix of the camera
	CustomViewInfo.FOV         = DesiredCameraFOV;
	CustomViewInfo.DesiredFOV  = DesiredCameraFOV;
	CustomViewInfo.AspectRatio = ViewportSize.X / ViewportSize.Y;
	
	// Set new projection matrix
	SceneCaptureComponent->bUseCustomProjectionMatrix = true;
	SceneCaptureComponent->CustomProjectionMatrix = CustomViewInfo.CalculateProjectionMatrix();
}

void USpectrumDebugWindowVTCamera::ControlledCameraMove(float DeltaTime, UWorld* World, const FVector2D& MouseDelta)
{
	if (!IsValid(World) || !IsValid(SceneCapture2D))
	{
		return;
	}

	ImGuiIO& IO = ImGui::GetIO();

	// Pitch and yaw
	const float Scalar = MouseSensitivity * DeltaTime;
	SceneCapture2D->AddActorWorldRotation({0, MouseDelta.X * Scalar, 0});
	SceneCapture2D->AddActorLocalRotation({-MouseDelta.Y * Scalar, 0, 0});

	// Translation
	FVector InputVector = FVector::ZeroVector;
	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		InputVector.X += 1.f;
	}
	if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		InputVector.X -= 1.f;
	}
	if (ImGui::IsKeyDown(ImGuiKey_A))
	{
		InputVector.Y -= 1.f;
	}
	if (ImGui::IsKeyDown(ImGuiKey_D))
	{
		InputVector.Y += 1.f;
	}
	// Up/Down
	if (ImGui::IsKeyDown(ImGuiKey_Space) || ImGui::IsKeyDown(ImGuiKey_E))
	{
		InputVector.Z += 1.f;
	}
	if (ImGui::IsKeyDown(ImGuiKey_C) || ImGui::IsKeyDown(ImGuiKey_Q))
	{
		InputVector.Z -= 1.f;
	}
	
	// Camera speed modification via mouse scroll while moving camera (same as UE4)
	CameraSpeed += IO.MouseWheel * ScrollScalar;
	CameraSpeed = FMath::Clamp(CameraSpeed, 0.01f, MAX_FLT);
	
	// Strip Z as we want to translate in world space not local
	float UpDown = InputVector.Z;
	InputVector.Z = 0.f;
	SceneCapture2D->AddActorWorldOffset({0.f, 0.f, UpDown * DeltaTime * CameraSpeed});

	FVector Acceleration = FVector::ZeroVector;
	if (bPlanarCameraMovement)
	{
		const FVector ForwardVector = SceneCapture2D->GetActorForwardVector();
		const FVector RightVector   = SceneCapture2D->GetActorRightVector();
		
		Acceleration = {
			ForwardVector.X * InputVector.X + RightVector.X * InputVector.Y,
			ForwardVector.Y * InputVector.X + RightVector.Y * InputVector.Y,
			0.0f
		};
		Acceleration = Acceleration.GetSafeNormal();
	}
	else
	{
		Acceleration = SceneCapture2D->GetActorRotation().RotateVector(InputVector);
	}

	SceneCapture2D->AddActorWorldOffset(Acceleration * DeltaTime * CameraSpeed);
}

void USpectrumDebugWindowVTCamera::DrawCameraControls(UWorld* World)
{
	if (!IsValid(SceneCapture2D))
	{
		return;
	}
	
	const USceneCaptureComponent2D* const SceneCaptureComponent2D = SceneCapture2D->GetCaptureComponent2D();
	if (!IsValid(SceneCaptureComponent2D))
	{
		return;
	}

	ImGui::Checkbox("Show Frustum", &bDrawCameraInWorld);

	// Camera type drop down
	const FString SelectedCameraTypeString = CameraTypeToString(CameraType);
	if (ImGui::BeginCombo("Camera Type", TCHAR_TO_ANSI(*SelectedCameraTypeString), ImGuiComboFlags_None))
	{
		for (ESpectrumRViewCameraType Val : TEnumRange<ESpectrumRViewCameraType>())
		{
			const FString CameraTypeString = CameraTypeToString(Val);
			
			const bool bIsSelected = (CameraType == Val);
			if (ImGui::Selectable(TCHAR_TO_ANSI(*CameraTypeString), bIsSelected))
			{
				CameraType = Val;
				OnCameraTypeChanged(World);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		
		ImGui::EndCombo();
	}
}

void USpectrumDebugWindowVTCamera::DrawCameraControlsInViewport(UWorld* World, const FVector2D& ViewportPos, const FVector2D& ViewportSize)
{
	// Move cursor to be more inline with the general window padding
	const ImGuiStyle& Style = ImGui::GetStyle();

	FVector2D NewViewportPos = ViewportPos;
	NewViewportPos.X += Style.WindowPadding.x;
	NewViewportPos.Y += Style.WindowPadding.x;

	FVector2D NewViewportSize = ViewportSize;
	NewViewportSize.X -= Style.WindowPadding.x * 2;
	NewViewportSize.Y -= Style.WindowPadding.y * 2;
	
	ImGui::SetCursorPos({(float)NewViewportPos.X, (float)NewViewportPos.Y});

	// Show a button to set the viewpoint perspective to the current player's eyes
	if (ImGui::Button(ICON_FA_ARROWS_TO_EYE "###ViewpointToPlayerEyes"))
	{
		SetCameraToLocalPlayersEye(World);
	}
	IMGUI_SIMPLE_TOOLTIP("Set camera to player's eyes");

	ImGui::SameLine();
	ImGui::SetNextItemWidth(75.f);
	ImGui::DragFloat("###MouseSens", &MouseSensitivity, 0.2f, 0, 0, ICON_FA_COMPUTER_MOUSE " %.0f");
	IMGUI_SIMPLE_TOOLTIP("Mouse sensitivity");
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(75.f);
	ImGui::DragFloat("###CameraSpeed", &CameraSpeed, 10.f, 0, 0, ICON_FA_PERSON_RUNNING " %.0f");
	IMGUI_SIMPLE_TOOLTIP("Camera speed");

	// Camera FoV slider
	ImGui::SameLine();
	ImGui::SetNextItemWidth(75.f);
	ImGui::DragFloat("###DesiredFov", &DesiredCameraFOV, 1, 1, 179, ICON_FA_VIDEO " %.0f");
	IMGUI_SIMPLE_TOOLTIP("FoV");

	ImGui::SameLine();
	ImGui::Checkbox(ICON_FA_TABLE_CELLS "###PlanarCameraMovement", &bPlanarCameraMovement);
	IMGUI_SIMPLE_TOOLTIP("Planar camera movement");
}

FString USpectrumDebugWindowVTCamera::CameraTypeToString(ESpectrumRViewCameraType InCameraType) const
{
	FString Result = "";
	
	switch(InCameraType)
	{
		case ESpectrumRViewCameraType::FreeCam:
		{
			Result = "Free Cam";
		}
		break;
		case ESpectrumRViewCameraType::Attached:
		{
			Result = "Attached";
		}
		break;
		
		default: ;
	}

	return Result;
}

void USpectrumDebugWindowVTCamera::OnCameraTypeChanged(UWorld* World)
{
	if (!IsValid(World))
	{
		return;	
	}

	if (!IsValid(SceneCapture2D))
	{
		return;
	}
	
	if (CameraType == ESpectrumRViewCameraType::Attached)
	{
		const APlayerController* const PlayerController = World->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			APawn* const Pawn = PlayerController->GetPawn();
			if (IsValid(Pawn))
			{
				const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, false);
				SceneCapture2D->AttachToActor(Pawn, AttachmentTransformRules);
			}
		}
	}
	else
	{
		const FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		SceneCapture2D->DetachFromActor(DetachmentTransformRules);
	}
}
