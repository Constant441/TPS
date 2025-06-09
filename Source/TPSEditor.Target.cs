// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class TPSEditorTarget : TargetRules
{
    [CommandLine("-Unoptimizedcode")]
    public bool Unoptimizedcode = true;
    
    public TPSEditorTarget(TargetInfo Target) : base(Target)
    {
        //Console.WriteLine("TPSEditorTarget UnoptimizedCode ----------------------> {0}", Unoptimizedcode);
        if (Unoptimizedcode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
        
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("TPS");
    }
}
