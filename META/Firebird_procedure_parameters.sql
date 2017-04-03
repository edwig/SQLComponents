SELECT '' as catalog_name
      ,(SELECT rdb$owner_name
          FROM rdb$procedures pro
         WHERE pro.rdb$procedure_name = par.rdb$procedure_name)
      ,par.rdb$procedure_name as procedure_name
      ,par.rdb$parameter_name as column_name
      ,par.rdb$parameter_number + par.rdb$parameter_type as ordinal_position
      ,(par.rdb$parameter_type * 3) + 1 as column_type

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
       END                           as data_type

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
       END                                        as column_size
      ,cast (fld.rdb$field_scale as smallint)*-1  as scale
      ,10                    as radix
      ,(coalesce(par.rdb$null_flag,0,0)-1)*-1     as nullable

      ,par.RDB$DESCRIPTION   as remarks
      ,par.RDB$DEFAULT_source as default_value
      ,CASE (coalesce(par.rdb$null_flag,0,0)-1)*-1
            WHEN 0 THEN 'NO'
            WHEN 1 THEN 'YES'
                   ELSE 'UNKNOWN'
        END                                      AS is_nullable
  FROM rdb$procedure_parameters par
      ,rdb$fields fld
 WHERE fld.rdb$field_name = par.rdb$field_source
   AND par.rdb$procedure_name = 'TESTINS'


UNION ALL

SELECT '' as catalog_name
      ,(SELECT rdb$owner_name
          FROM rdb$functions fun
         WHERE fun.rdb$function_name = par.rdb$function_name)
      ,par.rdb$function_name as procedure_name
      ,par.rdb$argument_name as column_name
      ,par.rdb$argument_position as ordinal_position
      ,CASE par.rdb$argument_name IS NULL
            WHEN TRUE THEN 3
                      ELSE 1
       END as column_type
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
       END                           as data_type

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
       END                                        as column_size
      ,cast (fld.rdb$field_scale as smallint)*-1  as scale
      ,10                    as radix
      ,(coalesce(par.rdb$null_flag,0,0)-1)*-1     as nullable

      ,par.rdb$description   as remarks
      ,par.rdb$default_source as default_value
      ,CASE (coalesce(par.rdb$null_flag,0,0)-1)*-1
            WHEN 0 THEN 'NO'
            WHEN 1 THEN 'YES'
                   ELSE 'UNKNOWN'
        END                                      AS is_nullable
  FROM rdb$function_arguments par
      ,rdb$fields fld
 WHERE fld.rdb$field_name = par.rdb$field_source
   AND par.rdb$function_name = 'TESTINS'