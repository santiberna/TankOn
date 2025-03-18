@echo off

:: cmake --preset Release
:: cmake --build build/Release

xcopy /Y /I /E "assets" "TankOn\assets"
xcopy /Y "build\Release\TankOn.exe" "TankOn\"

butler push TankOn santib/tankon:win

::rmdir /S /Q "TankOn\"