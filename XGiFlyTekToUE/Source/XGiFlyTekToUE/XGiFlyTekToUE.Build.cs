// Copyright Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGiFlyTekToUE : ModuleRules
{
	public XGiFlyTekToUE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {

			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
	
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "XGiFlyTekCore",
                "XGiFlyTekSTT",
                "XGiFlyTekTTS"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);
	}
}
