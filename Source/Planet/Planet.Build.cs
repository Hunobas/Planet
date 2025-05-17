// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class Planet : ModuleRules
{
	public Planet(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
		
        PublicIncludePaths.AddRange(new string[] {
	        Path.Combine(ModuleDirectory, "Content"),
	        Path.Combine(ModuleDirectory, "Content/Camera"),
	        Path.Combine(ModuleDirectory, "Content/Pawn"),
            Path.Combine(ModuleDirectory, "Content/Pawn/PawnMover"),
            Path.Combine(ModuleDirectory, "Content/Pawn/PawnModule"),
            Path.Combine(ModuleDirectory, "Content/Pawn/Enemy"),
            Path.Combine(ModuleDirectory, "Content/Wave"),
            Path.Combine(ModuleDirectory, "System")
        });
        
        PublicDefinitions.Add("ENABLE_DRAW_DEBUG=1");
        PublicDefinitions.Add("DEBUG=0");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
