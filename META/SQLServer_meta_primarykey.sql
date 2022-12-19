SELECT db_name()      AS table_catalog
      ,s.name         AS table_schema
      ,o.name         AS table_name
      ,c.name         AS column_name
      ,ic.key_ordinal AS key_sequence
      ,k.name         AS pk_name
  FROM sys.key_constraints k
       inner join sys.indexes i ON i.object_id = k.parent_object_id
       inner join sys.objects o ON i.object_id = o.object_id
       inner join sys.schemas s ON s.schema_id = o.schema_id
       inner join sys.index_columns ic ON (i.object_id = ic.object_id AND i.index_id   = ic.index_id)
       inner join sys.columns c        ON (c.object_id = o.object_id  AND ic.column_id = c.column_id)
 WHERE k.type           = 'PK'
   AND i.is_primary_key = 1
   AND s.name           = 'kdba'
   AND o.name           = 'KIW_AFL'
;

