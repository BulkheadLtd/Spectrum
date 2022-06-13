// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetTextureRT.h"

#include <Engine/TextureRenderTarget2D.h>

#include <ImGuiModule.h>

USpectrumWidgetTextureRT::USpectrumWidgetTextureRT()
{
}

USpectrumWidgetTextureRT::~USpectrumWidgetTextureRT()
{
	FImGuiModule::Get().ReleaseTexture(ImGuiTextureHandle);
}

void USpectrumWidgetTextureRT::Initialize(FVector2D Size)
{
	TextureRenderTarget = NewObject<UTextureRenderTarget2D>();
	if (IsValid(TextureRenderTarget))
	{
		TextureRenderTarget->InitAutoFormat(Size.X, Size.Y);
		TextureRenderTarget->UpdateResourceImmediate(true);
	}

	// Fetch texture from texture pool. If it does not exist (which is likely) then just make it and bind the render target texture
	const FName TextureName = FName(Guid.ToString());
	ImGuiTextureHandle = FImGuiModule::Get().FindTextureHandle(TextureName);
	if (!ImGuiTextureHandle.IsValid())
	{
		if (IsValid(TextureRenderTarget))
		{
			ImGuiTextureHandle = FImGuiModule::Get().RegisterTexture(TextureName, TextureRenderTarget, true);
		}
	}
}

TObjectPtr<UTextureRenderTarget2D> USpectrumWidgetTextureRT::GetTextureRenderTarget() const
{
	return TextureRenderTarget;
}

void USpectrumWidgetTextureRT::SetTextureSize(FVector2D NewSize)
{
	if (IsValid(TextureRenderTarget))
	{
		TextureRenderTarget->ResizeTarget(NewSize.X, NewSize.Y);
	}
}

bool USpectrumWidgetTextureRT::BeginInternal(const UWorld* World)
{
	if (ImGuiTextureHandle.IsValid() && IsValid(TextureRenderTarget))
	{
		// Use texture size if CustomSize not set
		ImVec2 Size = {(float)TextureRenderTarget->SizeX, (float)TextureRenderTarget->SizeY};
		if (CustomSize.SizeSquared() > 0.f)
		{
			Size = {(float)CustomSize.X, (float)CustomSize.Y};
		}
		
		ImGui::Image(ImGuiTextureHandle.GetTextureId(), Size);
	}
	
	return true;
}
