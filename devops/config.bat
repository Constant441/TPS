@echo off

rem Engine params
set EnginePath_4.27=D:\Programs\Unreal Engine\UE_4.27
set EnginePath_5.5=D:\Programs\Unreal Engine\UE_5.5

set UBTRelativePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=D:\Programs\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Current engine path
set CurrentEnginePath=%EnginePath_5.5%

rem Editor path (Note: path for UE4: "..\UE4Editor.exe")
set EditorPath=%CurrentEnginePath%\Engine\Binaries\Win64\UnrealEditor.exe

rem !! Engine version for packaging !!
set RunUATPath=%CurrentEnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
pushd %~dp0..
set ProjectRoot=%CD%
popd
set ProjectPureName=TPS
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath="%ProjectRoot%\Source"
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build Documentation
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// My game copyright
set EXTRA_MODULE_NAMES="%ProjectPureName%"
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%\Build\Windows\%ProjectPureName%.exe

rem Tests
set TestOutputLogPath=%ProjectRoot%\Build\Tests\Tests.log
set ReportOutputPath=%ProjectRoot%\Build\Tests
set UEAutomationContentPath=%CurrentEnginePath%\Engine\Content\Automation
set TestNames=TPSGame.Science
set OpenCPPCoveragePath=D:\Programs\OpenCppCoverage\OpenCppCoverage.exe