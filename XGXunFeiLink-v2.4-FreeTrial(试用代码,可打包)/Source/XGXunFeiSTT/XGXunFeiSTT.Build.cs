// Copyright 2023 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGXunFeiSTT : ModuleRules
{
    public XGXunFeiSTT(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "$(ModuleDir)/Public/AsyncNode",
                "$(ModuleDir)/Public/Subsystem",
                "$(ModuleDir)/Public/Type",
                "$(ModuleDir)/Public/Type/STT",

            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "XGXunFeiSTT/Public/AsyncNode",
                "XGXunFeiSTT/Public/Log",
                "XGXunFeiSTT/Public/Subsystem",
                "XGXunFeiSTT/Public/Thread",
                "XGXunFeiSTT/Public/Type",
                "XGXunFeiSTT/Public/Type/STT",

            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Json",
                "JsonUtilities",
                "XGXunFeiCore",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "AudioMixer",
                "AudioCapture",
                "WebSockets",



            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );

        //Need to make sure Android has Launch module.
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Launch"
                }
            );
        }
    }
}
