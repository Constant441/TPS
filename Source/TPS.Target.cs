// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class TPSTarget : TargetRules
{
    [CommandLine("-Unoptimizedcode")]
    public bool Unoptimizedcode = false;
    
    public TPSTarget(TargetInfo Target) : base(Target)
    {
        //Console.WriteLine("TPSTarget UnoptimizedCode ----------------------> {0}", Unoptimizedcode);
        if (Unoptimizedcode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
        
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("TPS");
    }
}
