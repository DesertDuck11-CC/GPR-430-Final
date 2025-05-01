@echo off
start cmd /k "cd /d out/build/x64-Debug && echo s | networkFinal.exe"
timeout /t 1 >nul
start cmd /k "cd /d out/build/x64-Debug && echo c | networkFinal.exe"