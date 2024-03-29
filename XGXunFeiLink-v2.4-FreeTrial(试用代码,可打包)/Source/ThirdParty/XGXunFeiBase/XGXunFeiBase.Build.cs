// Copyright 2023 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGXunFeiBase : ModuleRules
{
    protected virtual bool bPlatformSupportsSSL
    {
        get
        {
            return
                Target.Platform == UnrealTargetPlatform.Mac ||
                Target.Platform == UnrealTargetPlatform.Win64 ||
                Target.IsInPlatformGroup(UnrealPlatformGroup.Unix) ||
                Target.Platform == UnrealTargetPlatform.IOS ||
                Target.Platform == UnrealTargetPlatform.Android;
        }
    }

    protected virtual bool bUseDefaultSSLCert
    {
        get
        {
            return
                Target.Platform == UnrealTargetPlatform.Mac ||
                Target.Platform == UnrealTargetPlatform.IOS;
        }
    }



    public XGXunFeiBase(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDefinitions.Add("SSL_PACKAGE=1");

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
            }
        );

        if (bPlatformSupportsSSL)
        {
            PublicDefinitions.Add("WITH_SSL=1");
            PrivateDefinitions.Add("USE_DEFAULT_SSLCERT=" + (bUseDefaultSSLCert ? "1" : "0"));

            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

        }
        else
        {
            PublicDefinitions.Add("WITH_SSL=0");
        }

        UnsafeTypeCastWarningLevel = WarningLevel.Error;


    }
}
