SELECT TABLE_NAME
      ,COLUMN_NAME
  FROM information_schema.columns
 WHERE data_type = 'decimal'
   AND numeric_precision = 38
   AND numeric_scale     = 19;

SELECT 'alter table ' || TABLE_NAME || ' modify ' || COLUMN_NAME || ' integer;'
  FROM information_schema.columns
 WHERE data_type = 'decimal'
   AND numeric_precision = 38
   AND numeric_scale     = 19;

