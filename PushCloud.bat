@echo off
@echo Opschonen SQLComponents directories
@echo .

del SQLComponents.rar
del /q /s /f *.sdf
del /q /s /f *.ncb
del /q /s /f *.user
del /q /s /f *.VC.db
rmdir /q /s ipch
rmdir /q /s Debug
rmdir /q /s Release
rmdir /q /s TestSQL\Debug
rmdir /q /s TestSQL\Release
rmdir /q /s TestSQL\x64
rmdir /q /s SQLComponents\Debug
rmdir /q /s SQLComponents\Release
rmdir /q /s SQLComponents\x64

call prune.cmd

echo .
echo Ready cleaning up
echo .
echo Creating a RAR archive file
echo .
"C:\Program Files\Winrar\rar.exe" a SQLComponents.rar Documentation SQLComponents TestSQL testing.fdb *.bat *.sln
echo .
echo Ready making a RAR archive
echo .
robocopy . C:\Users\%USERNAME%\OneDrive\Documenten SQLComponents.rar /xo
echo .
echo Ready copying the archive to the cloud (OneDrive/SkyDrive)
pause