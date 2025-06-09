// Copyright Epic Games, Inc. All Rights Reserved.

using System.Linq;
using UnrealBuildTool;

public class TPS : ModuleRules
{
	public TPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"Json",
				"JsonUtilities",
				"UMG",
				"FunctionalTesting",
			}
		);

		PublicIncludePaths.Add("TPS");

		if (Target.ProjectDefinitions.Contains("UNOPTIMIZED_CODE"))
		{
			OptimizeCode = CodeOptimization.Never;
		}
	}
}