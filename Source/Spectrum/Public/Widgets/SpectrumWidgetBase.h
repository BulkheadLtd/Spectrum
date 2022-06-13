// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "SpectrumWidgetBase.generated.h"

// UE string to ImGui 
#define RAWCHARPTR(S) TCHAR_TO_ANSI(*S)

UCLASS(Abstract)
class SPECTRUM_API USpectrumWidgetBase : public UObject
{
	GENERATED_BODY()

public:

	// Sets the widget's size.
	// NOTE: It's up to the widget developer to respect this. Don't expect this to work for every widget!
	void SetCustomSize(FVector2D NewSize);

	bool Begin(const UWorld* World);
	void End();

	int32 GetImGuiGuid() const;

protected:

	// Override these to create custom behaviour
	virtual bool BeginInternal(const UWorld* World);
	virtual void EndInternal() {};
	
	FGuid Guid = FGuid::NewGuid();

	FVector2D CustomSize = FVector2D::ZeroVector;
};
