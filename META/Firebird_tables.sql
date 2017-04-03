SELECT CAST('' AS VARCHAR(31))             AS table_catalog
      ,CAST(rdb$owner_name AS VARCHAR(31)) AS table_schema
      ,rdb$relation_name                   AS table_name
      ,CASE rdb$relation_type
            WHEN 0 THEN CASE rdb$system_flag
                             WHEN 0 THEN 'TABLE'
                             WHEN 1 THEN 'SYSTEM TABLE'
                                    ELSE  'UNKNOWN'
                        END
            WHEN 1 THEN 'VIEW'
            WHEN 2 THEN 'TABLE'
            WHEN 3 THEN 'SYSTEM TABLE'
            WHEN 4 THEN 'GLOBAL TEMPORARY'
            WHEN 5 THEN 'LOCAL TEMPORARY'
                   ELSE 'UNKNOWN'
       END                                 AS table_type
      ,rdb$description                     AS remarks
       -- EXTRA
      ,rdb$view_source                     AS view_source
  FROM rdb$relations