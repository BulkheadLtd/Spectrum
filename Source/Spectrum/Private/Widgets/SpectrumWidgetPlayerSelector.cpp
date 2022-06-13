// Copyright BULKHEAD Limited. All Rights Reserved.

#include "Widgets/SpectrumWidgetPlayerSelector.h"

#include <Engine/World.h>
#include <GameFramework/GameStateBase.h>
#include <GameFramework/PlayerState.h>

bool USpectrumWidgetPlayerSelector::BeginInternal(const UWorld* World)
{
	if (IsValid(World))
	{
		const AGameStateBase* const GameState = World->GetGameState();
		if (IsValid(GameState))
		{
			const TArray<TObjectPtr<UObject>> PlayerObjects(GameState->PlayerArray);
			SetAvailableObjects(PlayerObjects);
		}
	}

	return Super::BeginInternal(World);
}

FString USpectrumWidgetPlayerSelector::GetObjectLabel(const TObjectPtr<UObject> Object) const
{
	FString Result;

	if (GetObjectLabelOverride.IsBound())
	{
		Result = GetObjectLabelOverride.Execute(Object);
	}
	else
	{
		const APlayerState* const Player = Cast<APlayerState>(Object);
		if (IsValid(Player))
		{
			Result = FString::Printf(TEXT("[%s] %s (%s)"), *Player->GetUniqueId().ToDebugString(),
			                         *Player->GetPlayerName(), *GetNameSafe(Player));
		}
		else
		{
			Result = GetNameSafe(Object);
		}
	}

	return Result;
}
