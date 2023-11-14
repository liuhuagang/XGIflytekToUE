// Copyright Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGiFlyTekCore : ModuleRules
{
    public XGiFlyTekCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
             "$(ModuleDir)/Public/Config",
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
               "XGiFlyTekCore/Public/Algorithm",
               "XGiFlyTekCore/Public/Config",
               "XGiFlyTekCore/Public/Type"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",

            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "DeveloperSettings",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }
}
