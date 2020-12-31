SELECT table_catalog  AS table_cat
      ,table_schema   AS table_schem
      ,table_name
      ,column_name
      ,case data_type
            when 'char'               then   1
            when 'varchar'            then  12
            when 'bigint'   then case locate('unsigned',column_type) > 0
                                 when true then -27   -- UBIGINT
                                           else -25   -- SBIGINT
                                 end
            when 'binary'             then  -2
            when 'bit'                then  -7
            when 'blob'               then  -4  -- longvarbinary
            when 'bool'               then  -7  -- bit
            when 'date'               then   9  -- date
            when 'datetime'           then  93  -- TYPE TIMESTAMP
            when 'decimal'            then   2  -- NUMERIC
            when 'double'             then   8
            when 'double precision'   then   8
            when 'enum'               then  12  -- VARCHAR
            when 'float'              then   7
            when 'int'      then case locate('unsigned',column_type) > 0
                                 when true then -18   -- ULONG
                                           else   4   -- SLONG
                                 end
            when 'integer'  then case locate('unsigned',column_type) > 0
                                 when true then -18   -- ULONG
                                           else   4   -- SLONG
                                 end
            when 'long varbinary'     then  -4  -- BLOB
            when 'long varchar'       then  -1
            when 'longblob'           then  -4
            when 'longtext'           then  -1
            when 'mediumblob'         then  -4
            when 'mediumint' then case locate('unsigned',column_type) > 0
                                  when true then -18   -- ULONG
                                            else   4   -- SLONG
                                  end
            when 'mediumtext'         then  -1
            when 'numeric'            then   2
            when 'real'               then   7
            when 'set'                then  12
            when 'smallint' then case locate('unsigned',column_type) > 0
                                 when true then -17   -- USMALLINT
                                           else   5   -- SMALLINT
                                 end
            when 'text'               then  -1
            when 'time'               then  92  -- TYPE TIME
            when 'timestamp'          then  93
            when 'tinyblob'           then  -3  -- varbinary
            when 'tinyint'  then case locate('unsigned',column_type) > 0
                                 when true then -16   -- UTINYINT
                                           else  -6   -- TINYINT
                                 end
            when 'tinytext'           then  -1
            when 'varbinary'          then  -3
       end  as data_type
      ,ucase(if(column_type like '%%(%%)%%',concat(substring(column_type,1,locate('(',column_type)-1),substring(column_type,1+locate(')',column_type))),column_type)) as type_name
      ,case when data_type = 'bit'    then (numeric_precision+7)/8
            when data_type in ('tinyint','smallint','mediumint','int','bigint','decimal') then numeric_precision
            when data_type = 'float'  then if(numeric_scale IS NULL, 7,numeric_precision)
            when data_type = 'double' then if(numeric_scale IS NULL,15,numeric_precision)
            when data_type = 'date'   then 10
            when data_type = 'time'   then  8
            when data_type = 'year'   then  4
            when data_type in ('timestamp','datetime') then 19
            else character_maximum_length
       end  as column_size
      ,case data_type
            when 'bit'        then 1
            when 'tinyint'    then 1
            when 'smallint'   then 2
            when 'int'        then 4
            when 'integer'    then 4
            when 'mediumint'  then 3
            when 'bigint'     then 20
            when 'real'       then 4
            when 'float'      then 8
            when 'double'     then 8
            when 'date'       then 6
            when 'time'       then 6
            when 'timestamp'  then 16
            when 'datetime'   then 16
            when 'guid'       then 16
            when 'year'       then 2
            when 'decimal'    then (numeric_precision + 2)
            else  character_octet_length
       end  as buffer_length
      ,nvl(numeric_scale,datetime_precision) AS decimal_digits
      ,if(character_octet_length is not null,null,10) as num_prec_radix
      ,if(data_type='timestamp',1,if(is_nullable='YES',1,if(extra='auto_increment',1,0))) as nullable
      ,column_comment     as remarks
      ,column_default     as column_def
      ,case data_type
            when 'date'      then 9   -- DATETIME
            when 'time'      then 9   -- DATETIME
            when 'datetime'  then 9   -- DATETIME
            when 'timestamp' then 9   -- DATETIME
       END AS sql_data_type
      ,case data_type
            when 'date'      then 1   -- DATE
            when 'time'      then 2   -- TIME
            when 'datetime'  then 3   -- CODE_TIMESTAMP
            when 'timestamp' then 3   -- CODE_TIMESTAMP
       END AS sql_datetime_sub
      ,character_octet_length as char_octet_length
      ,ordinal_position
      ,if(data_type = 'timestamp','YES',if(is_nullable = 'YES','YES',if(extra = 'auto_increment','YES','NO'))) as is_nullable
  FROM information_schema.columns
 WHERE table_name   = 'all_types';
   AND table_schema = 'pronto'
 ORDER BY table_cat
         ,table_schem
         ,table_name
         ,ordinal_position
;




SELECT * FROM infomation_schema.COLUMNS
WHERE table_name = 'all_types';

SELECT Nvl(NULL,20) FROM dual;