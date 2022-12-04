SELECT specific_catalog AS procedure_cat
      ,specific_schema  AS procedure_schem
      ,specific_name    AS procedure_name
      ,coalesce(parameter_name,'RETURNS')  AS column_name
      ,CASE parameter_mode
            WHEN 'IN'    THEN 1
            WHEN 'INOUT' THEN 2
            WHEN 'OUT'   THEN 3
            ELSE 4
       END AS column_type
      ,CASE data_type
            WHEN 'numeric'   THEN 2
            WHEN 'varchar'   THEN 1
            WHEN 'datetime'  THEN 11
            WHEN 'datetime2' THEN 11
            WHEN 'int'       THEN 4
       END AS data_type
      ,data_type AS type_name
      ,character_maximum_length AS column_size
      ,CASE data_type
            WHEN 'numeric'   THEN 36
            WHEN 'varchar'   THEN character_maximum_length
            WHEN 'datetime'  THEN 20
            WHEN 'datetime2' THEN 20
            WHEN 'int'       THEN 8
       END AS buffer_length
      ,numeric_scale           AS decimal_digits
      ,numeric_precision_radix AS num_prec_radix
      ,3  AS nullable
      ,null AS remarks
      ,null AS column_default
      ,null as sql_data_type
      ,null AS sql_datetime_sub
      ,character_octet_length AS char_octet_length
      ,ordinal_position
      ,'YES' AS is_nullable
  FROM information_schema.parameters
 WHERE specific_name = 'abs_gb_upd_proc';







                           _