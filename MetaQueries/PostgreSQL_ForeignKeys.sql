SELECT current_database() AS pktable_catalog
      ,pk_ns.nspname      AS pktable_schema
      ,pk_tbl.relname     AS pktable_name
      ,pk_col.attname     AS pktable_columnname
      ,current_database() AS fktable_catalog
      ,fk_ns.nspname      AS fktable_schema
      ,fk_tbl.relname     AS fktable_name
      ,fk_col.attname     AS fktable_columname
      ,pos.n              AS key_sequence
      ,CASE conf.confupdtype
           WHEN 'c' THEN 0  -- CASCADE
           WHEN 'n' THEN 1  -- SET NULL
           WHEN 'd' THEN 2  -- SET DEFAULT
           WHEN 'r' THEN 3  -- RESTRICT
           WHEN 'a' THEN 4  -- NO ACTION
       END AS update_rule
      ,CASE conf.confdeltype
           WHEN 'c' THEN 0
           WHEN 'n' THEN 1
           WHEN 'd' THEN 2
           WHEN 'r' THEN 3
           WHEN 'a' THEN 4
       END AS delete_rule
      ,conf.conname    AS fk_constraint_name
      ,pk_conf.conname AS pk_constraint_name
      ,CASE conf.condeferrable
           WHEN true THEN
               CASE conf.condeferred
                   WHEN true THEN 5  -- INITIALLY DEFERRED
                   ELSE 6            -- INITIALLY IMMEDIATE
               END
           ELSE 7                    -- NOT DEFERRABLE
       END AS deferrability
      ,case conf.confmatchtype WHEN 's' THEN 1
                               WHEN 'f' THEN 1
                               ELSE  0
                               END as match_option
      ,conf.condeferred   as initially_deferred
      ,1                  as enabled
  FROM pg_constraint conf
       JOIN pg_class     fk_tbl ON fk_tbl.oid = conf.conrelid
       JOIN pg_namespace fk_ns  ON fk_ns.oid  = fk_tbl.relnamespace
       JOIN pg_class     pk_tbl ON pk_tbl.oid = conf.confrelid
       JOIN pg_namespace pk_ns  ON pk_ns.oid  = pk_tbl.relnamespace
       JOIN pg_attribute fk_col ON fk_col.attrelid = conf.conrelid  AND fk_col.attnum = ANY (conf.conkey)
       JOIN pg_attribute pk_col ON pk_col.attrelid = conf.confrelid AND pk_col.attnum = ANY (conf.confkey)
       LEFT JOIN pg_constraint pk_conf ON pk_conf.conrelid = conf.confrelid AND pk_conf.contype IN ('p', 'u')
       JOIN generate_series(1, array_length(conf.conkey, 1)) AS pos(n) ON true
 WHERE conf.contype = 'f'
   AND fk_ns.nspname = 'idba'
   AND fk_tbl.relname = 'abs_gr'
-- AND fk_col.attname = 'column'
