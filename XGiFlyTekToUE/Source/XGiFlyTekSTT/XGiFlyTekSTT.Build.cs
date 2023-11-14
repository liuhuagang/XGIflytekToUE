// Copyright Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGiFlyTekSTT : ModuleRules
{
    public XGiFlyTekSTT(ReadOnlyTargetRules Target) : base(Target)
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
                "XGiFlyTekSTT/Public/Log",
                "XGiFlyTekSTT/Public/Subsystem",
                "XGiFlyTekSTT/Public/Thread",
                "XGiFlyTekSTT/Public/Type"
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
                "AudioMixer",
                "AudioCapture",
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
