// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "SpectrumWidgetBase.h"

#include "SpectrumWidgetPlotCollapser.generated.h"

class USpectrumWidgetRealtimePlot;

UCLASS()
class SPECTRUM_API USpectrumWidgetPlotCollapser : public USpectrumWidgetBase
{
	GENERATED_BODY()

public:

	// Begin USpectrumWidgetBase overrides
	virtual bool BeginInternal(const UWorld* World) override;
	// End USpectrumWidgetBase overrides

	virtual void SetTitlePrefix(const FString& InTitle);
	virtual void BindPlot(TWeakObjectPtr<USpectrumWidgetRealtimePlot> InRealtimePlot, const FName& InPlotName);

private:

	FName SummaryPlotName = "";
	FString TitlePrefix = "";

	TWeakObjectPtr<USpectrumWidgetRealtimePlot> RealtimePlot = nullptr;
};
