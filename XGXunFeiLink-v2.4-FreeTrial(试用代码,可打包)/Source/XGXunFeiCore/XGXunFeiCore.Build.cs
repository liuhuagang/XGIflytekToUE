// Copyright 2023 Xiao Gang. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class XGXunFeiCore : ModuleRules
{
    public XGXunFeiCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
             "$(ModuleDir)/Public/AsyncAction",
             "$(ModuleDir)/Public/Config",
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
               "XGXunFeiCore/Public/AsyncAction",
               "XGXunFeiCore/Public/Config",
               "XGXunFeiCore/Public/Type"
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
                "XGXunFeiBase",
                "Projects"
            }
            );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                        "Launch",
                }
            );
        }
    }
}
