@echo off

call "%~dp0\..\config.bat"

rem remove previous data folder
set TestDir=%~dp0
set TestDataDir=%~dp0data
rmdir /s /q "%TestDataDir%"

rem copy automation content
robocopy "%UEAutomationContentPath%" "%TestDataDir%" /E

rem install bower
call npm install -g bower

rem install bower pacakages
pushd "%TestDir%"
cd "%TestDataDir%"
call bower install "%TestDataDir%\bower.json"
popd

rem install simple server
call npm install http-server bower -g