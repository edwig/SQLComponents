SELECT database()                  AS pktable_catalog
      ,rc.unique_constraint_schema AS pktable_schema
      ,rc.referenced_table_name    AS pktable_name
      ,kcu.referenced_column_name  AS pkcolumn_name
      ,database()                  AS fktable_catalog
      ,rc.constraint_schema        AS fktable_schema
      ,rc.table_name               AS fktable_name
      ,kcu.column_name             AS fkcolumn_name
      ,kcu.position_in_unique_constraint AS key_sequence
      ,rc.update_rule
      ,rc.delete_rule
      ,rc.constraint_name          AS fk_constraint_name
      ,rc.unique_constraint_name   AS pk_constraint_name
      ,NULL                        AS deferrability
      ,match_option
      ,0                           AS initially_deferred
      ,1                           AS enabled
  FROM information_schema.referential_constraints rc
       JOIN information_schema.key_column_usage kcu
                         ON rc.constraint_name   = kcu.constraint_name
                        AND rc.constraint_schema = kcu.constraint_schema
                        AND rc.table_name        = kcu.table_name
        JOIN information_schema.table_constraints tc
                         ON tc.table_name        = rc.referenced_table_name
                        AND tc.constraint_schema = rc.constraint_schema
                        AND tc.constraint_name   = 'PRIMARY'
 WHERE rc.table_name   = 'order_details'
   AND rc.constraint_schema = 'northwind'


