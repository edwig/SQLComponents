SELECT *
  FROM information_schema.columns
 WHERE table_schema = 'kdba'
   AND table_name   = 'kiw_alt'
 ORDER BY 1,2,3
;
SELECT db_name() AS table_name
      ,table_schema
      ,lower(table_name) AS table_name
      ,column_name
      ,CASE data_type
            WHEN 'char'      THEN 1
            WHEN 'numeric'   THEN 2
            WHEN 'decimal'   THEN 3
            WHEN 'int'       THEN 4
            WHEN 'smallint'  THEN 5
            WHEN 'float'     THEN 6
            WHEN 'real'      THEN 7
            WHEN 'varchar'   THEN 12
            WHEN 'date'      THEN 91
            WHEN 'datetime'  THEN 93
            WHEN 'datetime2' THEN 93
            WHEN 'time'      THEN -154
            WHEN 'uniqueidentifier' THEN -11
            WHEN 'ntext'     THEN -10
            WHEN 'nvarchar'  THEN -9
            WHEN 'nchar'     THEN -8
            WHEN 'bit'       THEN -7
            WHEN 'tinyint'   THEN -6
            WHEN 'bigint'    THEN -5
            WHEN 'varbinary' THEN -4
            WHEN 'binary'    THEN -2
            WHEN 'text'      THEN -1
       END AS datatype
      ,data_type AS type_name
      ,CASE data_type
            WHEN 'char'      THEN character_maximum_length
            WHEN 'numeric'   THEN 18
            WHEN 'decimal'   THEN 17
            WHEN 'int'       THEN 10
            WHEN 'smallint'  THEN 5
            WHEN 'float'     THEN 53
            WHEN 'real'      THEN 24
            WHEN 'varchar'   THEN character_maximum_length
            WHEN 'date'      THEN 10
            WHEN 'datetime'  THEN 23
            WHEN 'datetime2' THEN 27
            WHEN 'time'      THEN 16
            WHEN 'uniqueidentifier' THEN -36
            WHEN 'ntext'     THEN character_maximum_length
            WHEN 'nvarchar'  THEN character_maximum_length
            WHEN 'nchar'     THEN character_maximum_length
            WHEN 'bit'       THEN 1
            WHEN 'tinyint'   THEN 3
            WHEN 'bigint'    THEN 19
            WHEN 'varbinary' THEN character_maximum_length
            WHEN 'binary'    THEN character_octet_length
            WHEN 'text'      THEN 2147483647
       END AS column_size
      ,CASE data_type
            WHEN 'char'      THEN character_maximum_length
            WHEN 'numeric'   THEN 20
            WHEN 'decimal'   THEN 19
            WHEN 'int'       THEN 4
            WHEN 'smallint'  THEN 2
            WHEN 'float'     THEN 8
            WHEN 'real'      THEN 4
            WHEN 'varchar'   THEN character_maximum_length
            WHEN 'date'      THEN 6
            WHEN 'datetime'  THEN 16
            WHEN 'datetime2' THEN 16
            WHEN 'time'      THEN 12
            WHEN 'uniqueidentifier' THEN 16
            WHEN 'ntext'     THEN character_octet_length
            WHEN 'nvarchar'  THEN character_octet_length
            WHEN 'nchar'     THEN character_octet_length
            WHEN 'bit'       THEN 1
            WHEN 'tinyint'   THEN 1
            WHEN 'bigint'    THEN 8
            WHEN 'varbinary' THEN character_octet_length
            WHEN 'binary'    THEN character_octet_length
            WHEN 'text'      THEN 2147483647
       END AS buffer_length
      ,numeric_scale AS decimal_digits
      ,numeric_precision_radix
      ,CASE is_nullable
            WHEN 'NO'  THEN 0
            WHEN 'YES' THEN 1
                       ELSE 2
       END AS nullable
      ,''  AS remarks
      ,column_default
      ,CASE data_type
            WHEN 'char'      THEN 1
            WHEN 'numeric'   THEN 2
            WHEN 'decimal'   THEN 3
            WHEN 'int'       THEN 4
            WHEN 'smallint'  THEN 5
            WHEN 'float'     THEN 6
            WHEN 'real'      THEN 7
            WHEN 'varchar'   THEN 12
            WHEN 'date'      THEN 9
            WHEN 'datetime'  THEN 9
            WHEN 'datetime2' THEN 93
            WHEN 'time'      THEN -154
            WHEN 'uniqueidentifier' THEN -11
            WHEN 'ntext'     THEN -10
            WHEN 'nvarchar'  THEN -9
            WHEN 'nchar'     THEN -8
            WHEN 'bit'       THEN -7
            WHEN 'tinyint'   THEN -6
            WHEN 'bigint'    THEN -5
            WHEN 'varbinary' THEN -3
            WHEN 'binary'    THEN -2
            WHEN 'text'      THEN -1
       END AS sql_datatype
      ,CASE data_type
            WHEN 'date'      THEN 1
            WHEN 'datetime'  THEN 3
                             ELSE 0
       END AS datatype_sub
      ,character_octet_length
      ,ordinal_position
      ,is_nullable
  FROM tempdb.information_schema.columns
 WHERE table_schema = 'dbo'
   AND table_name   = '##myresults'
 ORDER BY 1,2,3
;

