// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class declassify_door : ModuleRules
{
	public declassify_door(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"EnhancedInput" 
		});
		
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
