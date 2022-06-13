// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWidgetBase.h"

#include "SpectrumWidgetFVector.generated.h"

UCLASS()
class SPECTRUM_API USpectrumWidgetFVector : public USpectrumWidgetBase
{
	GENERATED_BODY()

public:

	virtual bool Begin(const UWorld* World, const FVector& InVector);

	virtual void SetLabel(const FString& NewLabel);

	virtual void SetEditable(bool bEditable);

protected:
	
	virtual bool BeginInternal(const UWorld* World) override;

private:

	FVector Vector = FVector::ZeroVector;

	FString Label;

	bool bIsEditable = true;
};
