-- TABLE PRIVILEGES

SELECT sys_context('USERENV','DB_NAME') AS table_catalog
      ,pri.table_schema
      ,pri.table_name
      ,pri.grantor
      ,pri.grantee
      ,pri.privilege
      ,pri.grantable
  FROM all_tab_privs pri
 WHERE table_schema = 'KDBA'
   AND table_name   = 'KIW_ENH'
UNION
SELECT sys_context('USERENV','DB_NAME') AS table_catalog
      ,'KDBA'
      ,'KIW_ENH'
      ,'_SYSTEM'
      ,'KDBA'
      ,name
      ,'YES'
  FROM table_privilege_map
 ORDER BY 1,2,3,5,6,4
;

-- COLUMNS

SELECT *
  FROM dba_col_privs;

SELECT * FROM objauth$;