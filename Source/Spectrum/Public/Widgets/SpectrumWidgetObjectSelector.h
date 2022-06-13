// Copyright BULKHEAD Limited. All Rights Reserved.

#pragma once

#include "Widgets/SpectrumWidgetBase.h"

#include "SpectrumWidgetObjectSelector.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(FString, FSpectrumObjectLabelDelegate, const TObjectPtr<UObject>)

UCLASS()
class SPECTRUM_API USpectrumWidgetObjectSelector : public USpectrumWidgetBase
{
	GENERATED_BODY()

public:

	// Begin USpectrumWidgetBase overrides
	virtual bool BeginInternal(const UWorld* World) override;
	// End USpectrumWidgetBase overrides

	void SetAvailableObjects(const TArray<TObjectPtr<UObject>>& InObjects);
	void SetSelectedObject(const TObjectPtr<UObject> InObject);
	void SetSelectedObjectIndex(const int32 InObjectIdx);

	TArray<TObjectPtr<UObject>> GetAvailableObjects() const;
	TObjectPtr<UObject> GetSelectedObject() const;
	int32 GetSelectedObjectIndex() const;

	template<typename T, TEMPLATE_REQUIRES(TIsDerivedFrom<T, UObject>::IsDerived)>
	TObjectPtr<T> GetSelectedObject() const;

	FSpectrumObjectLabelDelegate GetObjectLabelOverride;

protected:

	virtual FString GetObjectLabel(const TObjectPtr<UObject> Object) const;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> AvailableObjects;

	int32 SelectedObjectIdx = INDEX_NONE;
};

template<typename T, typename TEnableIf<TIsDerivedFrom<T, UObject>::IsDerived, int>::type>
TObjectPtr<T> USpectrumWidgetObjectSelector::GetSelectedObject() const
{
	return Cast<T>(GetSelectedObject());
}
