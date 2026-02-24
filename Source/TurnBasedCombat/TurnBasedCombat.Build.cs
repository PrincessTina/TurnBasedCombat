// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TurnBasedCombat : ModuleRules
{
	public TurnBasedCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TurnBasedCombat",
			"TurnBasedCombat/Variant_Strategy",
			"TurnBasedCombat/Variant_Strategy/UI",
			"TurnBasedCombat/Variant_TwinStick",
			"TurnBasedCombat/Variant_TwinStick/AI",
			"TurnBasedCombat/Variant_TwinStick/Gameplay",
			"TurnBasedCombat/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
