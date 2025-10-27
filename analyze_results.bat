@echo off
REM =====================================
REM   CVRP Results Analyzer - Windows BAT
REM   Autor: Adrian Sośniak
REM   Data: 2025-10-27
REM =====================================

set SCRIPT=analyze_cvrp_results.py
set OUTPUT_DIR=report_output

echo =====================================
echo [INFO] Uruchamiam analizę wyników CVRP
echo =====================================

REM Sprawdź czy Python jest dostępny
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

REM Uruchom analizę
echo [INFO] Uruchamianie skryptu Python...
python "%SCRIPT%"

REM Sprawdź wynik
if %errorlevel% neq 0 (
    echo [ERROR] Coś poszło nie tak podczas analizy wyników.
    pause
    exit /b
)

echo.
echo =====================================
echo [OK] Analiza zakończona pomyślnie!
echo Wyniki znajdują się w folderze: %OUTPUT_DIR%
echo - summary.txt
echo - summary_auto.tex
echo - *.png (wykresy)
echo =====================================
echo.

pause
