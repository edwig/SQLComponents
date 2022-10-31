@echo off
@echo Opschonen SQLComponents directories
@echo .

del SQLComponents.rar
del /q /s /f *.sdf
del /q /s /f *.ncb
del /q /s /f *.user
del /q /s /f *.VC.db
rmdir /q /s .vs
rmdir /q /s Debug
rmdir /q /s Release
rmdir /q /s bin
rmdir /q /s lib
rmdir /q /s BaseLibrary\x64
rmdir /q /s BaseLibrary\Debug
rmdir /q /s BaseLibrary\Release
rmdir /q /s SQLComponents\Debug
rmdir /q /s SQLComponents\Release
rmdir /q /s SQLComponents\x64
rmdir /q /s SQLConnections\Debug
rmdir /q /s SQLConnections\Release
rmdir /q /s SQLConnections\x64
rmdir /q /s SQLMigrate\x64
rmdir /q /s SQLMigrate\Debug
rmdir /q /s SQLMigrate\Release
rmdir /q /s TestSQL\Debug
rmdir /q /s TestSQL\Release
rmdir /q /s TestSQL\x64
rmdir /q /s UnitTest\Debug
rmdir /q /s UnitTest\Release
rmdir /q /s UnitTest\x64
rmdir /q /s TestResults

call prune.cmd

@echo .
@echo Ready cleaning up SQLComponents
