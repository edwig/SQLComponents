SELECT database()   AS catalog_name
      ,table_schema AS schema_name
      ,table_name
      ,non_unique
      ,''           AS index_qualifier
      ,index_name
      ,CASE index_type
            WHEN 'BTREE'     THEN 0
            WHEN 'CLUSTERED' THEN 1
            WHEN 'HASHED'    THEN 2
            ELSE 3
       END  AS index_type
      ,seq_in_index
      ,column_name
      ,'A'          AS ascending
      ,cardinality
      ,'' AS pages
      ,'' AS filter_condition
  FROM information_schema.statistics
 WHERE TABLE_SCHEMA = 'northwind'
   AND TABLE_NAME = 'employee_privileges'
 ORDER BY index_name,seq_in_index
;


