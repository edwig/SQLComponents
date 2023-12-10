@echo Making a backup
@mkdir c:\develop\sqlcomponents\Backups
"C:\Program Files\Firebird\Firebird_3_0\gbak" -b -g -V -user SYSDBA -pas altijd testing  c:\develop\sqlcomponents\Backups\testing_backup.fbk  -Y C:\Develop\SQLComponents\Backups\testing_backup.log
"C:\Program Files\Firebird\Firebird_3_0\gbak" -b -g -V -user SYSDBA -pas altijd test_utf c:\develop\sqlcomponents\Backups\test_utf_backup.fbk -Y C:\Develop\SQLComponents\Backups\test_utf_backup.log
