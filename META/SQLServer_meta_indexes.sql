SELECT db_name() AS catalog_name
      ,s.name    AS table_schema
      ,o.name    AS table_name
      ,CASE i.is_unique
            WHEN 1 THEN 0
                   ELSE 1
       END       AS non_unique
      ,s.name    AS index_qualifier
      ,i.name    AS index_name
      ,1         AS TYPE
      ,x.index_column_id AS ordinal_position
      ,c.name    AS column_name
      ,CASE x.is_descending_key
            WHEN 0 THEN 'A'
            WHEN 1 THEN 'D'
                   ELSE ''
       END       AS asc_or_desc
      ,y.rowcnt  AS cardinality
      ,y.dpages  AS pages
      ,CAST(i.filter_definition AS varchar) AS filter
  FROM sys.indexes i
       INNER JOIN sys.objects       o ON  o.object_id = i.object_id
       INNER JOIN sys.index_columns x ON (x.object_id = i.object_id AND x.index_id  = i.index_id)
       INNER JOIN sys.columns       c ON (c.object_id = o.object_id AND x.column_id = c.column_id)
       INNER JOIN sys.schemas       s ON  s.schema_id = o.schema_id
       INNER JOIN sys.sysindexes    y ON (i.object_id = y.id AND i.name = y.name)
 WHERE s.name = 'idba'
   AND o.name = 'dbs_ei';

SELECT id
      ,name
      ,rowcnt
      ,dpages
  FROM sys.sysindexes
 WHERE name = 'pk_dbs_ei';

SELECT *
  FROM sys.indexes
 WHERE name = 'pk_dbs_ei';