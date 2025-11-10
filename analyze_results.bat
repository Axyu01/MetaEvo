@echo off
REM =====================================
REM   CVRP Results Analyzer - Windows BAT
REM =====================================

set SCRIPT=main.py
set OUTPUT_DIR=report_output

echo =====================================
echo [INFO] Uruchamiam analize wynikow CVRP
echo =====================================

REM Sprawdz czy Python jest dostepny
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Python nie jest zainstalowany lub nie jest w PATH.
    echo Zainstaluj Pythona 3 i spróbuj ponownie.
    pause
    exit /b
)

REM Tworzenie katalogu wynikowego
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

REM Uruchom analize
echo [INFO] Uruchamianie skryptu Python...
python "%SCRIPT%"

echo.
echo =====================================
echo [OK] Analiza zakonczona pomyslnie!
echo Wyniki znajduja sie w folderze: %OUTPUT_DIR%
echo =====================================
echo.

pause
