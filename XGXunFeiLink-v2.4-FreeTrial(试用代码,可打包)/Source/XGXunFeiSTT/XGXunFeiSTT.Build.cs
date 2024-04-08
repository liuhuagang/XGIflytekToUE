// Copyright 2023 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGXunFeiSTT : ModuleRules
{
    public XGXunFeiSTT(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "$(ModuleDir)/Public/Type",
                "$(ModuleDir)/Public/Subsystem",
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "XGXunFeiSTT/Public/Log",
                "XGXunFeiSTT/Public/Subsystem",
                "XGXunFeiSTT/Public/Thread",
                "XGXunFeiSTT/Public/Type"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "XGXunFeiCore"
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
                "Json",
                "JsonUtilities",

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
