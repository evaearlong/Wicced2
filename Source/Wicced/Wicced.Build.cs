// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Wicced : ModuleRules
{
	public Wicced(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
            "GameplayTags",
            "GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Wicced",
			"Wicced/Variant_Platforming",
			"Wicced/Variant_Platforming/Animation",
			"Wicced/Variant_Combat",
			"Wicced/Variant_Combat/AI",
			"Wicced/Variant_Combat/Animation",
			"Wicced/Variant_Combat/Gameplay",
			"Wicced/Variant_Combat/Interfaces",
			"Wicced/Variant_Combat/UI",
			"Wicced/Variant_SideScrolling",
			"Wicced/Variant_SideScrolling/AI",
			"Wicced/Variant_SideScrolling/Gameplay",
			"Wicced/Variant_SideScrolling/Interfaces",
			"Wicced/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
