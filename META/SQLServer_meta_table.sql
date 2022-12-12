CREATE TABLE #dbs_ses_global
(
   one INT
  ,two VARCHAR(10)
);

CREATE TABLE ##dbs_ses_local
(
   one INT
  ,two VARCHAR(200)
);

SELECT db_name() AS table_catalog
      ,s.name    AS table_schema
      ,o.name    AS table_name
      ,CASE o.type
            WHEN 'U'  THEN 'TABLE'
            WHEN 'TT' THEN 'TABLE'
            WHEN 'V'  THEN 'VIEW'
            WHEN 'SN' THEN 'SYNONYM'
            WHEN 'S'  THEN 'SYSTEM TABLE'
                      ELSE 'UNKNOWN'
       END AS table_type
      ,CAST (e.value AS VARCHAR(Max)) AS remarks
      ,null AS tablespace
      ,0    AS temporary
  FROM sys.objects o
            INNER JOIN sys.schemas s ON o.schema_id = s.schema_id
       LEFT OUTER JOIN sys.extended_properties e ON
                     ( e.major_id  = o.object_id
                    AND e.minor_id = 0
                    AND e.class    = 1
                    AND e.name     = N'MS_Description')
 WHERE o.type IN ('U','TT','V','SN','S')
   AND e.name     = N'MS_Description'
   AND e.minor_id = 0
   AND e.class    = 1
   AND s.name     = 'idba'
   AND o.name     = 'dbs_ses'
UNION ALL
SELECT db_name() AS table_catalog
      ,'idba'    AS table_schema
      ,SubString(o.name,3,len(o.name)-2) AS table_name
      ,'LOCAL TEMPORARY' AS table_type
      ,null AS remarks
      ,null AS tablespace
      ,1    AS temporary
  FROM tempdb.sys.objects o
 WHERE o.type      = 'U'
   AND o.schema_id = 1
   AND o.name      LIKE '##dbs_ses%'
UNION ALL
SELECT db_name() AS table_catalog
      ,'idba'    AS table_schema
      ,substring(o.name,2,charindex('___',o.name)-2) AS test
      ,'GLOBAL TEMPORARY' AS table_type
      ,NULL AS remarks
      ,null AS tablespace
      ,1    AS temporary
  FROM tempdb.sys.objects o
 WHERE o.type      = 'U'
   AND o.schema_id = 1
   AND o.name LIKE '#dbs_ses___%'

 ORDER BY 1,2,3
;

EXEC sp_addextendedproperty
     @name = N'MS_Description',
     @Value = N'Example global temporary table',
     @level0type = N'SCHEMA',@level0name = N'sys',
     @level1type = N'TABLE', @level1name = N'dbs_ses_global'