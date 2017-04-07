SELECT cast('' as varchar(255))                   as table_cat                -- 1  - VARCHAR
      ,cast(tbl.rdb$owner_name    as varchar(31)) as table_schem		          -- 2  - VARCHAR
      ,cast(col.rdb$relation_name as varchar(31)) as table_name	              -- 3  - VARCHAR NOT NULL
      ,cast(col.rdb$field_name    as varchar(31)) as column_name		          -- 4  - VARCHAR NOT NULL
      ,CASE fld.rdb$field_type
            WHEN 7  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE 5
                              END
            WHEN 8  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE 4
                              END
            WHEN 10 THEN 7
            WHEN 12 THEN 10
            WHEN 13 THEN 13
            WHEN 14 THEN 1
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE -5
                         END
            WHEN 23 THEN -7
            WHEN 27 THEN 8
            WHEN 35 THEN 11
            WHEN 37 THEN 12
            WHEN 261 THEN CASE fld.rdb$field_sub_type
                               WHEN 0 THEN -4
                               WHEN 1 THEN -1
                                      ELSE -10
                          END
                     ELSE 0
       END                           as data_type 			          -- 5  - SMALLINT NOT NULL
      ,CASE fld.rdb$field_type
            WHEN 7  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 'NUMERIC'
                              WHEN 2 THEN 'DECIMAL'
                                     ELSE 'SMALLINT'
                              END
            WHEN 8  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 'NUMERIC'
                              WHEN 2 THEN 'DECIMAL'
                                     ELSE 'INTEGER'
                              END
            WHEN 10 THEN 'FLOAT'
            WHEN 12 THEN 'DATE'
            WHEN 13 THEN 'TIME'
            WHEN 14 THEN 'CHAR'
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 'NUMERIC'
                              WHEN 2 THEN 'DECIMAL'
                                     ELSE 'BIGINT'
                         END
            WHEN 23 THEN 'BOOLEAN'
            WHEN 27 THEN 'DOUBLE PRECISION'
            WHEN 35 THEN 'TIMESTAMP'
            WHEN 37 THEN 'VARCHAR'
            WHEN 261 THEN CASE fld.rdb$field_sub_type
                               WHEN 0 THEN 'BLOB SUB_TYPE 0'
                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'
                                      ELSE 'BLOB'
                          END
                     ELSE 'UNKNOWN'
       END                                        as type_name		            -- 6  - VARCHAR NOT NULL
      ,CASE fld.rdb$field_type
            WHEN 7  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN fld.rdb$field_precision
                              WHEN 2 THEN fld.rdb$field_precision
                                     ELSE 5
                              END
            WHEN 8  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN fld.rdb$field_precision
                              WHEN 2 THEN fld.rdb$field_precision
                                     ELSE 11
                              END
            WHEN 10 THEN 12
            WHEN 12 THEN 10
            WHEN 13 THEN 13
            WHEN 14 THEN fld.rdb$field_length
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN fld.rdb$field_precision
                              WHEN 2 THEN fld.rdb$field_precision
                                     ELSE 22
                         END
            WHEN 23 THEN 1
            WHEN 27 THEN 20
            WHEN 35 THEN 26
            WHEN 37 THEN fld.rdb$field_length
            WHEN 261 THEN 2147483648
       END                                        as column_size      		    -- 7  - INTEGER
      ,cast(fld.rdb$field_length as integer)      as buffer_length	          -- 8  - INTEGER
      ,cast (fld.rdb$field_scale as smallint)*-1  as decimal_digits		        -- 9  - SMALLINT
      ,10                                         as num_prec_radix           -- 10 - SMALLINT
      ,(coalesce(col.rdb$null_flag,0,0)-1)*-1     as nullable        				  -- 11 - SMALLINT NOT NULL
      ,cast (col.rdb$description as varchar(512)) as remarks                  -- 12 - VARCHAR
      ,cast (col.rdb$default_source as varchar(512)) as column_def         		-- 13 - VARCHAR
      ,CASE fld.rdb$field_type
            WHEN 7  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE 5
                              END
            WHEN 8  THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE 4
                              END
            WHEN 10 THEN 7
            WHEN 12 THEN 9
            WHEN 13 THEN 10
            WHEN 14 THEN 1
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 1 THEN 2
                              WHEN 2 THEN 2
                                     ELSE -5
                         END
            WHEN 23 THEN -7
            WHEN 27 THEN 8
            WHEN 35 THEN 11
            WHEN 37 THEN 12
            WHEN 261 THEN CASE fld.rdb$field_sub_type
                               WHEN 0 THEN -4
                               WHEN 1 THEN -1
                                      ELSE -10
                          END
                     ELSE 0
       END                                         as sql_data_type 			    -- 14 - SMALLINT NOT NULL
      ,CAST(0 AS SMALLINT)                         as sql_datetime_sub        -- 15 - SMALLINT
      ,fld.rdb$field_length / rdb$character_length as char_octet_length    		-- 16 - INTEGER
      ,col.rdb$field_position + 1                  as ordinal_position				-- 17 - INTEGER NOT NULL
      ,CASE (coalesce(col.rdb$null_flag,0,0)-1)*-1
            WHEN 0 THEN 'NO'
            WHEN 1 THEN 'YES'
                   ELSE 'UNKNOWN'
       END                                         AS is_nullable             -- 18 - VARCHAR
 FROM rdb$relation_fields  col
      ,rdb$fields          fld
      ,rdb$relations       tbl
 WHERE col.rdb$field_source  = fld.rdb$field_name
   AND col.rdb$relation_name = tbl.rdb$relation_name
   AND tbl.rdb$relation_name = 'ALL_TYPES'
 ORDER BY col.rdb$field_position