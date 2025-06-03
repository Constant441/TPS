@echo off

set VersionSelector=D:\Programs\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe
set ProjectName=TPS.uproject
set ProjectPath=%CD%\%ProjectName%
set EnginePath=D:\Programs\Unreal Engine\UE_4.27
set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool.exe

"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"
"%EnginePath%\%UBTRelativePath%" -projectfiles -progress -project="%ProjectPath%"

pause
