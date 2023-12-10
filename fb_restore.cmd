@echo restore from backup
@del testing.fdb
@del test_utf.fdb
"C:\Program Files\Firebird\Firebird_4_0\gbak" -c -V -user SYSDBA -pas altijd c:\develop\sqlcomponents\Backups\testing_backup.fbk  C:\Develop\SQLComponents\testing.fdb  -Y C:\Develop\SQLComponents\Backups\testing_restore.log
"C:\Program Files\Firebird\Firebird_4_0\gbak" -c -V -user SYSDBA -pas altijd c:\develop\sqlcomponents\Backups\test_utf_backup.fbk C:\Develop\SQLComponents\test_utf.fdb -Y C:\Develop\SQLComponents\Backups\test_utf_restore.log


