// Copyright BULKHEAD Limited. All Rights Reserved.

#include "SpectrumSettings.h"

#include <Engine/Engine.h>

#include "SpectrumModule.h"

TObjectPtr<USpectrumSettings> USpectrumSettings::Get()
{
	return FSpectrumModule::Get().GetSpectrumSettings();
}

void USpectrumSettings::OnWorldBegin(UWorld* World)
{
	if (GEngine)
	{
		FString Command = FString("r.DebugSafeZone.MaxDebugTextStringsPerActor ") + FString::FromInt(DebugDrawStringMaxCount);
		GEngine->Exec(World, *Command);
	}
}

void USpectrumSettings::Save()
{
	SaveConfig();

	// React to config saves
	if (GEngine)
	{
		FString Command = FString("r.DebugSafeZone.MaxDebugTextStringsPerActor ") + FString::FromInt(DebugDrawStringMaxCount);
		GEngine->Exec(GetWorld(), *Command);
	}
}
