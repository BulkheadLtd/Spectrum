// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include <Templates/SubclassOf.h>

#include "Widgets/SpectrumWidgetObjectSelector.h"

#include "SpectrumWidgetActorSelector.generated.h"

class AActor;

UCLASS()
class SPECTRUM_API USpectrumWidgetActorSelector : public USpectrumWidgetObjectSelector
{
	GENERATED_BODY()

public:

	// Begin USpectrumWidgetBase overrides
	virtual bool BeginInternal(const UWorld* World) override;
	// End USpectrumWidgetBase overrides

	void SetSupportedClass(const TSubclassOf<AActor> InClass);

	template<typename T = AActor, TEMPLATE_REQUIRES(TIsDerivedFrom<T, AActor>::IsDerived)>
	TObjectPtr<T> GetSelectedActor() const;

protected:

	TSubclassOf<AActor> SupportedClass;
};

template<typename T, typename TEnableIf<TIsDerivedFrom<T, AActor>::IsDerived, int>::type>
TObjectPtr<T> USpectrumWidgetActorSelector::GetSelectedActor() const
{
	return Cast<T>(GetSelectedObject());
}
