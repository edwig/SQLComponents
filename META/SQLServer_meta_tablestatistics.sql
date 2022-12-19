
SELECT db_name() AS catalog_name
      ,s.name    AS schema_name
      ,t.name    AS table_name
      ,1         AS non_unique
      ,''        AS index_name
      ,0         AS TYPE -- table_stats
      ,0         AS ordinal_position
      ,''        AS column_name
      ,''        AS asc_or_desc
      ,p.rows    AS cardinality
      ,Max(a.data_pages) AS pages
  FROM sys.tables t
       INNER JOIN sys.partitions p       ON t.object_id    = p.object_id
       INNER JOIN sys.allocation_units a ON p.partition_id = a.container_id
       INNER JOIN sys.schemas s          ON t.schema_id    = s.schema_id
 WHERE s.name = 'idba'
   AND t.NAME = 'dbs_ei'
 GROUP BY s.name,t.name,p.rows
 ORDER BY 1,2,3,4
;

