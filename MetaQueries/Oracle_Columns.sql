ALTER session SET optimizer_mode = all_rows;
ALTER SYSTEM FLUSH buffer_cache;

DROP   TABLE kdba.all_types;
CREATE TABLE kdba.all_types
(
  column1_char6        CHAR(6)
 ,column2_varchar2     VARCHAR2(20)
 ,column3_nchar        NCHAR(12)
 ,column4_nvarchar2    NVARCHAR2(200)
 ,column5_number       NUMBER
 ,column6_integer      INTEGER
 ,column7_float        FLOAT
 ,colunn8_real         REAL
 ,column9_short        NUMBER(5)
 ,column10_binfloat    binary_float
 ,column11_bindouble   binary_double
 ,column12_date        DATE
 ,column13_timestamp   TIMESTAMP
 ,column14_timestamptz TIMESTAMP WITH TIME ZONE
 ,column15_intvalmonth INTERVAL YEAR TO MONTH
 ,column16_intvalsec   INTERVAL DAY TO SECOND
 ,column17_clob        CLOB
 ,column18_long        LONG
 ,column19_blob        BLOB
 ,column20_nclob       NCLOB
 ,column21_number      NUMBER(18,4)
);

COMMENT ON COLUMN all_types.column6_integer IS 'This is a REAL integer';

SELECT sys_context('USERENV','DB_NAME') AS col_catalog
      ,col.owner                        AS col_owner
      ,col.table_name                   AS col_table
      ,col.column_name                  AS col_column_name
      ,CASE SubStr(col.data_type,1,8)
            WHEN 'CHAR'          THEN  1
            WHEN 'VARCHAR2'      THEN 12
            WHEN 'NCHAR'         THEN -8
            WHEN 'NVARCHAR'      THEN -9
            WHEN 'NUMBER'        THEN CASE data_scale
                                           WHEN 0   THEN 4
                                                    ELSE 2
                                      END
            WHEN 'FLOAT'         THEN CASE data_precision
                                           WHEN 63  THEN 7
                                                    ELSE 8
                                      END
            WHEN 'BINARY_F'      THEN  7
            WHEN 'BINARY_D'      THEN  8
            WHEN 'DATE'          THEN 93
            WHEN 'TIMESTAM'      THEN 93
            WHEN 'INTERVAL'      THEN -4
            WHEN 'CLOB'          THEN -1
            WHEN 'LONG'          THEN -1
            WHEN 'BLOB'          THEN -4
            WHEN 'NCLOB'         THEN -10
                                 ELSE  0
       END                             AS col_datatype
      ,col.data_type                   AS col_typename
      ,CASE SubStr(col.data_type,1,8)
            WHEN 'LONG'      THEN 2147483647
            WHEN 'CLOB'      THEN 2147483647
            WHEN 'BLOB'      THEN 2147483647
            WHEN 'NCLOB'     THEN 2147483647
            WHEN 'DATE'      THEN 19
            WHEN 'TIMESTAM'  THEN 19
            WHEN 'INTERVAL'  THEN 52
            WHEN 'BINARY_D'  THEN 15
            WHEN 'BINARY_F'  THEN  7
            WHEN 'NCHAR'     THEN char_col_decl_length
            WHEN 'NVARCHAR'  THEN char_col_decl_length
            WHEN 'NUMBER'    THEN NVL(data_precision,38)
                             ELSE col.data_length
       END                             AS col_columnsize
      ,CASE SubStr(col.data_type,1,8)
            WHEN 'LONG'     THEN 2147483647
            WHEN 'CLOB'     THEN 2147483647
            WHEN 'BLOB'     THEN 2147483647
            WHEN 'NCLOB'    THEN 2147483647
            WHEN 'DATE'     THEN 16
                            ELSE col.data_length
       END                             AS col_bufferlength
      ,col.data_scale                  AS col_decimaldigits
      ,CASE col.data_type
            WHEN 'NUMBER'        THEN 10
            WHEN 'FLOAT'         THEN 10
            WHEN 'BINARY_FLOAT'  THEN 10
            WHEN 'BINARY_DOUBLE' THEN 10
                                 ELSE  0
       END                             AS col_numradix
      ,CASE nullable
            WHEN 'N'      THEN 0   -- SQL_NO_NULLS
            WHEN 'Y'      THEN 1   -- SQL_NULLABLE
                          ELSE 2   -- SQL_NULLABLE_UNKNOWN
       END                             AS col_nullable
      ,com.comments                    AS col_remarks
      ,col.data_default                AS col_default
      ,CASE SubStr(col.data_type,1,8)
            WHEN 'CHAR'          THEN  1
            WHEN 'VARCHAR2'      THEN 12
            WHEN 'NCHAR'         THEN -8
            WHEN 'NVARCHAR'      THEN -9
            WHEN 'NUMBER'        THEN CASE data_scale
                                           WHEN 0   THEN 4
                                                    ELSE 2
                                      END
            WHEN 'FLOAT'         THEN CASE data_precision
                                           WHEN 63  THEN 7
                                                    ELSE 8
                                      END
            WHEN 'BINARY_F'      THEN  7
            WHEN 'BINARY_D'      THEN  8
            WHEN 'DATE'          THEN  9
            WHEN 'TIMESTAM'      THEN  9
            WHEN 'INTERVAL'      THEN -4
            WHEN 'CLOB'          THEN -1
            WHEN 'LONG'          THEN -1
            WHEN 'BLOB'          THEN -4
            WHEN 'NCLOB'         THEN -10
                                 ELSE  0
       END                             AS col_datatype3
      ,CASE SubStr(col.data_type,1,8)
            WHEN 'DATE'      THEN 3
            WHEN 'TIMESTAM'  THEN 3
                             ELSE 0
       END                             AS col_subdatatype
      ,CASE col.data_type
            WHEN 'CHAR'      THEN data_length
            WHEN 'VARCHAR2'  THEN data_length
            WHEN 'NCHAR'     THEN data_length
            WHEN 'NVARCHAR2' THEN data_length
                             ELSE 0
       END                             AS col_octetlength
      ,col.column_id                   AS col_position
      ,CASE col.nullable
            WHEN 'Y' THEN 'YES'
                     ELSE 'NO'
       END                            AS col_isnullable
  FROM all_tab_columns col
       LEFT OUTER JOIN all_col_comments com ON (col.owner       = com.owner
                                            AND col.table_name  = com.table_name
                                            AND col.column_name = com.column_name)
 WHERE col.owner      = 'KDBA'
   AND col.table_name = 'ALL_TYPES'
 ORDER BY 1,2,3,17

;

SELECT *
  FROM all_tab_columns col
 WHERE owner      = 'KDBA'
   AND table_name = 'ALL_TYPES'
 ORDER BY column_id
;




CREATE TABLE test_types
(
  field1    CHAR(10)           NOT NULL
 ,field2    VARCHAR2(200)
 ,field3    NCHAR(10)          NOT NULL
 ,field4    NCHAR varying(200) DEFAULT 'Génèpëlletje' NOT NULL
 ,field5    NUMBER
 ,field6    NUMBER(14)
 ,field7    NUMBER(16,2)
 ,field8    INTEGER            NOT NULL
 ,field9    FLOAT
 ,field10   REAL               NOT NULL
 ,field11   binary_float
 ,field12   binary_double
 ,field13   DATE               DEFAULT SYSDATE NOT null
 ,field14   TIMESTAMP(3)
 ,field15   TIMESTAMP(6) WITH TIME zone
 ,field16   TIMESTAMP WITH LOCAL TIME zone
 ,field17   INTERVAL YEAR(5) TO MONTH
 ,field18   INTERVAL DAY(6) TO SECOND(3) NOT NULL
 ,field19   CLOB
 ,field20   RAW (2000)
 ,field21   LONG
 ,field22   BLOB
 ,field23   NCLOB
);

DROP TABLE test_types;

SELECT *
  FROM all_tab_columns
 WHERE owner = 'KDBA'
   AND table_name = 'TEST_TYPES';