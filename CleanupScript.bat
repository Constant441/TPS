@echo off

:: Текущая директория
set "current_path=%~dp0"

:: Список объектов для удаления
set "objects_to_delete=Binaries Intermediate DerivedDataCache .vs .vsconfig"

:: Список расширений файлов для удаления
set "extensions_to_delete=sln user"

:: Разделитель
set separator=--------------------------------------------------------

:: Приветственное сообщение
echo %separator%
echo CLEANUP SCRIPT - DELETE OPTIONAL FILES AND FOLDERS
echo %separator%
echo Current directory: %current_path%
echo %separator%

:: Перебираем объекты для удаления
for %%f in (%objects_to_delete%) do (
    if exist "%current_path%%%f" (
        if exist "%current_path%%%f\" (
            echo [SUCCESS] %%f deleted.
            rd /s /q "%current_path%%%f"
        ) else (
            echo [SUCCESS] %%f deleted.
            del /q "%current_path%%%f"
        )
    ) else (
        echo [INFO] %%f not found.
    )
)

:: Удаляем файлы с указанными расширениями
for %%e in (%extensions_to_delete%) do (
    for %%f in ("%current_path%\*.%%e") do (
        if exist "%%~f" (
            del /q "%%~f"
            echo [SUCCESS] %%~nxf deleted.
        ) else (
            echo [INFO] No *.%%e files found.
        )
    )
)

echo %separator%
echo Cleanup completed.
echo %separator%

:: Ожидание 10 секунды перед закрытием консоли
timeout /t 10
