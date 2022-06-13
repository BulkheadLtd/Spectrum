// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include <ImGuiTextureHandle.h>

#include "SpectrumWidgetBase.h"

#include "SpectrumWidgetTextureRT.generated.h"

class UTextureRenderTarget2D;

UCLASS()
class SPECTRUM_API USpectrumWidgetTextureRT : public USpectrumWidgetBase
{
	GENERATED_BODY()

public:

	USpectrumWidgetTextureRT();
	~USpectrumWidgetTextureRT();

	void Initialize(FVector2D Size);
	
	TObjectPtr<UTextureRenderTarget2D> GetTextureRenderTarget() const;

	// Sets the underlying texture size. Ensure this is a multiple of 2 or crash might ensue
	void SetTextureSize(FVector2D NewSize);

protected:
	
	virtual bool BeginInternal(const UWorld* World) override;

private:

	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget = nullptr;

	FImGuiTextureHandle ImGuiTextureHandle;
};
