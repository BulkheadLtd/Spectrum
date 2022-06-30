// Copyright BULKHEAD Limited. All Rights Reserved.

using UnrealBuildTool;

public class Spectrum : ModuleRules
{
	public Spectrum(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreOnline",

			"ApplicationCore",
			"SlateCore",
			"Slate",
			"InputCore",

			// These two are required to pull RHI frame times
			"RenderCore",
			"RHI",

			// For rendering the gameplay tags tree in the built in debugger
			"GameplayTags",

			"ImGui"
		});

		PublicDefinitions.Add("WITH_SPECTRUM=1");
	}
}
