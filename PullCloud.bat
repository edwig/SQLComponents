@echo off
@echo Cleaning SQLComponents directories
@echo .

del SQLComponents.rar
del /q /s /f *.sdf
del /q /s /f *.ncb
del /q /s /f *.sln
del /q /s /f *.VC.db
rmdir /q /s Documentation
rmdir /q /s SQLComponents
rmdir /q /s TestSQL
rmdir /q /s Debug
rmdir /q /s Release
rmdir /q /s x64
echo .
echo Ready cleaning up
echo .
echo Getting the archive from the cloud
copy C:\Users\%USERNAME%\OneDrive\Documenten\SQLComponents.rar .
echo .
echo Ready getting the RAR archive
echo .
"C:\Program Files\Winrar\rar.exe" x -o+ SQLComponents.rar
echo .
echo Ready getting the archive from the (OneDrive/SkyDrive) cloud
echo .
pause