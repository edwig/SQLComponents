SELECT Trim(DBINFO('dbname')) AS procedure_catalog
      ,Trim(pro.owner)        AS procedure_schema
      ,Trim(pro.procname)     AS procedure_name
      ,Trim(col.paramname)    AS parameter_name
      ,paramattr              AS column_type
      ,CASE paramtype
            WHEN 0  THEN 1 -- char
            WHEN 1  THEN 5 -- smallint
            WHEN 2  THEN 4 -- integer
            WHEN 3  THEN 8 -- float
            WHEN 4  THEN 7 -- float
            WHEN 5  THEN 2 -- decimal
            WHEN 6  THEN 4 -- serial
            WHEN 7  THEN 9 -- date
            WHEN 8  THEN 2 -- money/decimal
            WHEN 10 THEN 11 -- timestmap
            WHEN 11 THEN -6 -- byte
            WHEN 12 THEN -3 -- text
            WHEN 14 THEN -89 -- interval
            WHEN 15 THEN -9  -- nchar
            WHEN 17 THEN -5  -- bigint
            WHEN 18 THEN -5  -- bigint serial
            WHEN 41 THEN -4  -- blob
            WHEN 43 THEN -1  -- lvarchar
            WHEN 45 THEN -7  -- bit/boolean
            WHEN 52 THEN -5  -- bigint
            WHEN 53 THEN -5  -- bigserial
                    ELSE 0   -- unknown
       END AS dataype
      ,CASE paramtype
            WHEN 0  THEN 'CHAR'
            WHEN 1  THEN 'SMALLINT'
            WHEN 2  THEN 'INTEGER'
            WHEN 3  THEN 'DOUBLE'
            WHEN 4  THEN 'FLOAT'
            WHEN 5  THEN 'DECIMAL'
            WHEN 6  THEN 'INTEGER'
            WHEN 7  THEN 'DATE'
            WHEN 8  THEN 'DECIMAL'
            WHEN 10 THEN 'TIMESTAMP'
            WHEN 11 THEN 'BYTE'
            WHEN 12 THEN 'VARCHAR'
            WHEN 14 THEN 'INTERVAL'
            WHEN 15 THEN 'WCHAR'
            WHEN 17 THEN 'BIGINT'
            WHEN 18 THEN 'SERIAL'
            WHEN 41 THEN 'LONGVARBINARY'
            WHEN 43 THEN 'WVARCHAR'
            WHEN 45 THEN 'BIT'
            WHEN 52 THEN 'BIGINT'
            WHEN 53 THEN 'BIGINT'
                    ELSE 'UNKNOWN'
       END AS type_name
      ,CASE paramtype
            WHEN 0  THEN paramlen  -- char
            WHEN 1  THEN 5 -- smallint
            WHEN 2  THEN 11 -- integer
            WHEN 3  THEN 13 -- float
            WHEN 4  THEN 21 -- float
            WHEN 5  THEN 40 -- decimal
            WHEN 6  THEN 11 -- serial
            WHEN 7  THEN 10 -- date
            WHEN 8  THEN 40 -- money/decimal
            WHEN 10 THEN 22 -- timestmap
            WHEN 11 THEN paramlen -- byte
            WHEN 12 THEN paramlen -- text
            WHEN 14 THEN 20 -- interval
            WHEN 15 THEN paramlen  -- nchar
            WHEN 17 THEN 22  -- bigint
            WHEN 18 THEN 22  -- bigint serial
            WHEN 41 THEN paramlen  -- blob
            WHEN 43 THEN paramlen  -- lvarchar
            WHEN 45 THEN 1  -- bit/boolean
            WHEN 52 THEN 22  -- bigint
            WHEN 53 THEN 22  -- bigserial
                    ELSE 0   -- unknown
      ,END AS column_size
      ,CASE paramtype
            WHEN 0  THEN paramlen -- char
            WHEN 1  THEN 2 -- smallint
            WHEN 2  THEN 4 -- integer
            WHEN 3  THEN 8 -- double
            WHEN 4  THEN 4 -- float
            WHEN 5  THEN Trunc(paramlen/256) -- decimal
            WHEN 6  THEN 4 -- serial
            WHEN 7  THEN Trunc(paramlen/256) -- date
            WHEN 8  THEN Trunc(paramlen/256) -- money/decimal
            WHEN 10 THEN Trunc(paramlen/256) -- timestmap
            WHEN 11 THEN paramlen     -- byte
            WHEN 12 THEN paramlen     -- text
            WHEN 14 THEN Trunc(paramlen/256) -- interval
            WHEN 15 THEN paramlen  -- nchar
            WHEN 17 THEN 8   -- bigint
            WHEN 18 THEN 8   -- bigint serial
            WHEN 41 THEN paramlen  -- blob
            WHEN 43 THEN paramlen  -- lvarchar
            WHEN 45 THEN 1   -- bit/boolean
            WHEN 52 THEN 8   -- bigint
            WHEN 53 THEN 8   -- bigserial
                    ELSE 0   -- unknown
       END  AS buffer_length
      ,Mod(paramlen,16) AS decimal_digits
      ,10       AS numRadix
      ,1        AS nullable
      ,''       AS remarks
      ,''       AS defaults
      ,Trunc(Mod(paramlen,256)/16)  AS datatype3
      ,Mod(paramlen,16)             AS subtype3
      ,paramlen AS octetlength
      ,paramid  AS position
      ,'YES'    AS isnullable
  FROM sysproccolumns col
      ,sysprocedures  pro
 WHERE pro.procid   = col.procid
   AND pro.procname = 'detail_amount';


