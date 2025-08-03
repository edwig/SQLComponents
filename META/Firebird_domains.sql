CREATE domain "Rabat" AS DECIMAL(12,4)
      Default 0.1
      NOT NULL
      CHECK(value = 0.0 OR (0.05 <= VALUE AND value <= 0.30));

create domain firstname as varchar(255)
      not null
      check((substring(value FROM 2)      = lower(substring(value from 2))) and
           (substring(value from 1 FOR 1) = UPPer(substring(value from 1 for 1))));


create domain "Hunderd" as INTEGER default 100
      check (value >= 100 AND value < 200);

SELECT Trim(dbs.mon$database_name) AS catalog_name
      ,Trim(fld.rdb$owner_name)    AS schema_name
      ,Trim(fld.rdb$field_name)    AS type_name
      ,'D'                         AS typeusage
      ,1                           AS ordinal
      ,''                          AS attributename
      ,CASE fld.rdb$field_type
            WHEN 7  THEN 'SMALLINT'
            WHEN 8  THEN 'INTEGER'
            WHEN 9  THEN 'QUAD'
            WHEN 10 THEN 'FLOAT'
            WHEN 11 THEN 'D_FLOAT'
            WHEN 12 THEN 'DATE'
            WHEN 13 THEN 'TIME'
            WHEN 14 THEN 'CHAR'
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 0 THEN 'BIGINT'
                              WHEN 1 THEN 'NUMERIC'
                              WHEN 2 THEN 'DECIMAL'
                                     ELSE 'BIGINT'
                         END
            WHEN 23 THEN 'BOOLEAN'
            WHEN 27 THEN 'DOUBLE PRECISION'
            WHEN 35 THEN 'TIMESTAMP'
            WHEN 37 THEN 'VARCHAR'
            WHEN 40 THEN 'CSTRING'
            WHEN 261 THEN CASE fld.rdb$field_sub_type
                               WHEN 0 THEN 'BLOB SUB_TYPE 0'
                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'
                                      ELSE 'BLOB'
                          END
                     ELSE 'UNKNOWN'
       END ||
       CASE WHEN rdb$field_type = 16 AND rdb$field_sub_type IN (1,2)
            THEN '(' || rdb$field_precision || ',' || (rdb$field_scale * -1) || ')'
            ELSE '' END ||
       CASE WHEN fld.rdb$field_type IN (14,37,40)
            THEN '(' || rdb$field_length || ')'
            ELSE ''
       END
      ,rdb$null_flag               AS notnullable
      ,rdb$default_source          AS defaultvalue
      ,rdb$validation_source       AS domaincheck
      ,rdb$description             AS remarks
      ,'<@>'                       AS source
  FROM mon$database dbs
      ,rdb$fields fld
 WHERE fld.rdb$owner_name = 'SYSDBA'
   AND fld.rdb$field_name IN ('Rabat','FIRSTNAME','Hunderd')
;


SELECT 'CREATE DOMAIN "' || Trim(fld.rdb$owner_name) || '"."' || Trim(fld.rdb$field_name) || '" AS ' ||
       CASE fld.rdb$field_type
            WHEN 7  THEN 'SMALLINT'
            WHEN 8  THEN 'INTEGER'
            WHEN 9  THEN 'QUAD'
            WHEN 10 THEN 'FLOAT'
            WHEN 11 THEN 'D_FLOAT'
            WHEN 12 THEN 'DATE'
            WHEN 13 THEN 'TIME'
            WHEN 14 THEN 'CHAR'
            WHEN 16 THEN CASE fld.rdb$field_sub_type
                              WHEN 0 THEN 'BIGINT'
                              WHEN 1 THEN 'NUMERIC'
                              WHEN 2 THEN 'DECIMAL'
                                     ELSE 'BIGINT'
                         END
            WHEN 23 THEN 'BOOLEAN'
            WHEN 27 THEN 'DOUBLE PRECISION'
            WHEN 35 THEN 'TIMESTAMP'
            WHEN 37 THEN 'VARCHAR'
            WHEN 40 THEN 'CSTRING'
            WHEN 261 THEN CASE fld.rdb$field_sub_type
                               WHEN 0 THEN 'BLOB SUB_TYPE 0'
                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'
                                      ELSE 'BLOB'
                          END
                     ELSE 'UNKNOWN'
       END ||
       CASE WHEN rdb$field_type = 16 AND rdb$field_sub_type IN (1,2)
            THEN '(' || rdb$field_precision || ',' || (rdb$field_scale * -1) || ')'
            ELSE '' END ||
       CASE WHEN fld.rdb$field_type IN (14,37,40)
            THEN '(' || rdb$field_length || ')'
            ELSE ''
       END ||
       COALESCE(' ' || rdb$default_source,'') || ' ' ||
       CASE rdb$null_flag
            WHEN 1 THEN 'NOT NULL '
            ELSE ''
       END || rdb$validation_source AS source
  FROM rdb$fields fld
 WHERE fld.rdb$owner_name = 'SYSDBA'
   AND fld.rdb$field_name IN ('Rabat','FIRSTNAME','Hunderd')
;

