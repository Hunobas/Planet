// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class Planet : ModuleRules
{
	public Planet(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "UMG", "EnhancedInput", "Niagara" });
		
        PublicIncludePaths.AddRange(new string[] {
	        Path.Combine(ModuleDirectory, "Content"),
	        Path.Combine(ModuleDirectory, "Content/Camera"),
	        Path.Combine(ModuleDirectory, "Content/Pawn"),
            Path.Combine(ModuleDirectory, "Content/Pawn/PawnMover"),
            Path.Combine(ModuleDirectory, "Content/Pawn/PawnModule"),
            Path.Combine(ModuleDirectory, "Content/Pawn/Enemy"),
			Path.Combine(ModuleDirectory, "Content/Pawn/Weapon"),
            Path.Combine(ModuleDirectory, "Content/Pawn/UpdateStretegy"),
            Path.Combine(ModuleDirectory, "Content/Wave"),
            Path.Combine(ModuleDirectory, "System"),
            Path.Combine(ModuleDirectory, "System/Reward/Data"),
            Path.Combine(ModuleDirectory, "System/Reward/Manager"),
            Path.Combine(ModuleDirectory, "System/Reward/Applicator"),
            Path.Combine(ModuleDirectory, "System/Reward/Type"),
            Path.Combine(ModuleDirectory, "Data/DataTable"),
            Path.Combine(ModuleDirectory, "Data/Assets"),
            Path.Combine(ModuleDirectory, "UI")
        });
        
        PublicDefinitions.Add("DEBUG=1");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
