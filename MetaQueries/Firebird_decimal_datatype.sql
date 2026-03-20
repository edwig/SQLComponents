select rf.RDB$FIELD_NAME        as field_name
      ,fl.RDB$FIELD_TYPE        as datatype
      ,fl.RDB$FIELD_SUB_TYPE    as sub_type
      ,fl.RDB$FIELD_LENGTH      as binary_length
      ,fl.RDB$FIELD_PRECISION   as num_precision
      ,(0 - fl.rdb$field_scale) as num_scale
  from RDB$RELATION_FIELDS rf
       INNER JOIN rdb$fields  fl ON fl.rdb$field_name = rf.RDB$FIELD_SOURCE
 where rdb$relation_name =  'ONE';
 
 

 
