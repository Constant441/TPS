@echo off

call "%~dp0\..\config.bat"

rem build sources
call "%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-archivedirectory="%ArchivePath%" ^
-build -cook

rem run tests
"%EditorPath%" "%ProjectPath%" -ExecCmds="Automation RunTests %TestNames%; Quit"^
 -log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem copy test artifacts
set TestDir=%~dp0
set TestDataDir=%~dp0data
robocopy "%TestDataDir%" "%ReportOutputPath%" /E

rem start local server and show report
set Port=8081
set Localhost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%Localhost%"
call http-server -p="%Port%" 
popd