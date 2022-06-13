// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetActorSelector.h"

#include <EngineUtils.h>
#include <GameFramework/Actor.h>

bool USpectrumWidgetActorSelector::BeginInternal(const UWorld* World)
{
	if (IsValid(World))
	{
		TArray<TObjectPtr<UObject>> ActorObjects;
		for (FActorIterator It(World, SupportedClass); It; ++It)
		{
			ActorObjects.Add(*It);
		}

		SetAvailableObjects(ActorObjects);
	}

	return Super::BeginInternal(World);
}

void USpectrumWidgetActorSelector::SetSupportedClass(const TSubclassOf<AActor> InClass)
{
	SupportedClass = InClass;
}
