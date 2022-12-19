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
      ,CASE e.name
            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(Max))
            ELSE ''
       END  AS remarks
      ,null AS tablespace
      ,0    AS temporary
  FROM sys.objects o
            INNER JOIN sys.schemas s  ON o.schema_id = s.schema_id
       LEFT OUTER JOIN sys.extended_properties e ON
                     ((e.major_id  = o.object_id OR e.major_id IS null)
                   AND e.minor_id = 0
                   AND e.class = 1)
 WHERE o.type IN ('U','TT','SN','S')
UNION ALL
SELECT db_name()
      ,''
      ,SubString(o.name,3,len(o.name) - 2)
      ,'LOCAL TEMPORARY'
      ,null
      ,null
      ,1
  FROM tempdb.sys.objects o
 WHERE o.type      = 'U'
   AND o.schema_id = 1
   AND o.name      LIKE '##%'
UNION ALL
SELECT db_name()
      ,''
      ,substring(o.name,2,charindex('___',o.name) - 2)
      ,'GLOBAL TEMPORARY'
      ,null
      ,null
      ,1
  FROM tempdb.sys.objects o
 WHERE o.type      = 'U'
   AND o.schema_id = 1
   AND o.name LIKE '#\_\_\_%' ESCAPE '\' ORDER BY 1,2,3