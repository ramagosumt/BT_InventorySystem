// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BachelorThesis : ModuleRules
{
	public BachelorThesis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "ProceduralMeshComponent" });
	}
}
