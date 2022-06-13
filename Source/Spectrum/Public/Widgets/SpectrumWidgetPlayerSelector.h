// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include "Widgets/SpectrumWidgetObjectSelector.h"

#include "SpectrumWidgetPlayerSelector.generated.h"

class APlayerState;

UCLASS()
class SPECTRUM_API USpectrumWidgetPlayerSelector : public USpectrumWidgetObjectSelector
{
	GENERATED_BODY()

public:

	// Begin USpectrumWidgetBase overrides
	virtual bool BeginInternal(const UWorld* World) override;
	// End USpectrumWidgetBase overrides

	template<typename T = APlayerState, TEMPLATE_REQUIRES(TIsDerivedFrom<T, APlayerState>::IsDerived)>
	TObjectPtr<T> GetSelectedPlayer() const;

protected:

	// Begin USpectrumWidgetObjectSelector override
	virtual FString GetObjectLabel(const TObjectPtr<UObject> Object) const override;
	// End USpectrumWidgetObjectSelector override
};

template<typename T, typename TEnableIf<TIsDerivedFrom<T, APlayerState>::IsDerived, int>::type>
TObjectPtr<T> USpectrumWidgetPlayerSelector::GetSelectedPlayer() const
{
	return Cast<T>(GetSelectedObject());
}
