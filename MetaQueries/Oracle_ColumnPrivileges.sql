-- COLUMN PRIVILEGES

GRANT UPDATE(column1_char6,column2_varchar2) ON kdba.all_types TO k2b;
GRANT UPDATE(column12_date,column13_timestamp) ON kdba.all_types TO k2b;
GRANT REFERENCES(column1_char6,column2_varchar2) ON kdba.all_types TO k2b WITH GRANT option;
GRANT REFERENCES(column12_date,column13_timestamp) ON kdba.all_types TO k2b WITH GRANT OPTION;

SELECT * FROM all_col_privs;


SELECT sys_context('USERENV','DB_NAME') AS table_catalog
      ,pri.table_schema
      ,pri.table_name
      ,pri.column_name
      ,pri.grantor
      ,pri.grantee
      ,pri.privilege
      ,pri.grantable
  FROM all_col_privs pri
 WHERE table_schema = 'KDBA'
   AND table_name = 'ALL_TYPES'
 ORDER BY 1,2,3,4,6,5,7