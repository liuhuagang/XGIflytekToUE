// Copyright 2023 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGKeDaXunFeiSound : ModuleRules
{
    public XGKeDaXunFeiSound(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "$(ModuleDir)/Public/Type"



            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {

                "XGKeDaXunFeiSound/Public/Actor",
                "XGKeDaXunFeiSound/Public/Algorithm",
                "XGKeDaXunFeiSound/Public/Algorithm",
                "XGKeDaXunFeiSound/Public/Component",
                "XGKeDaXunFeiSound/Public/Config",
                "XGKeDaXunFeiSound/Public/Core",
                "XGKeDaXunFeiSound/Public/Thread",
                "XGKeDaXunFeiSound/Public/Type",


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
                "Slate",
                "SlateCore",
                "WebSockets",
                "AudioMixer",
                "SignalProcessing",
                "AudioCapture",
                "Json",
                "DeveloperSettings",
                "JsonUtilities",
                "AudioCapture",
                "AudioCaptureEditor",
                "AudioCaptureRtAudio"


            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }
}
