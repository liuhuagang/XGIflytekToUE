// Copyright 2023 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGXunFeiTTS : ModuleRules
{
    public XGXunFeiTTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
             "$(ModuleDir)/Public/AsyncNode",
             "$(ModuleDir)/Public/Type",
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "XGXunFeiTTS/Public/AsyncNode",
                "XGXunFeiTTS/Public/Log",
                "XGXunFeiTTS/Public/Type"
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
                "WebSockets",
                "Json",
                "JsonUtilities",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }
}
