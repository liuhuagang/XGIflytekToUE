// Copyright Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGiFlyTekTTS : ModuleRules
{
    public XGiFlyTekTTS(ReadOnlyTargetRules Target) : base(Target)
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
                "XGiFlyTekTTS/Public/AsyncNode",
                "XGiFlyTekTTS/Public/Log",
                "XGiFlyTekTTS/Public/Type"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "XGiFlyTekCore"
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
