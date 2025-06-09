@echo off

call "%~dp0\..\config.bat"

set Before=public bool Unoptimizedcode = false;
set After=public bool Unoptimizedcode = true;
set File=%SourceCodePath%\%ProjectPureName%Editor.Target.cs
powershell -Command "$f='%File%'; $b='%Before%'; $a='%After%'; (Get-Content $f) -replace $b, $a | Set-Content $f"


rem build sources
call "%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-archivedirectory="%ArchivePath%" ^
-build -cook -ubtargs="-Unoptimizedcode"

goto:EOF
rem run tests
set TestRunner="%EditorPath%" "%ProjectPath%" -ExecCmds="Automation RunTests %TestNames%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem run code coverage
set ExportType=cobertura:%ReportOutputPath%\Coverage\CodeCoverageReport.xml
"%OpenCPPCoveragePath%" --modules="%ProjectRoot%" --sources="%SourceCodePath%" ^
--excluded_sources="%SourceCodePath%\TPS\Tests" --export_type="%ExportType%" -v -- %TestRunner%

rem clean obsolete artifacts
del /q LastCoverageResults.log
powershell -Command "$f='%File%'; $b='%After%'; $a='%Before%'; (Get-Content $f) -replace $b, $a | Set-Content $f"


rem copy test artifacts
set TestDir=%~dp0
set TestDataDir=%~dp0data
robocopy "%TestDataDir%" "%ReportOutputPath%" /E

rem start local server and show report
set Port=8081
set Localhost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%Localhost%"
start "" "%Localhost%\Coverage\CodeCoverageReport\index.html"
call http-server -p="%Port%" 
popd