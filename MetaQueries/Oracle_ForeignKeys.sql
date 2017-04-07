ALTER session SET optimizer_mode = CHOOSE;
ALTER SYSTEM FLUSH buffer_cache;

--begin
--    dbms_stats.gather_fixed_objects_stats;
--    dbms_stats.gather_dictionary_stats;
--end;

SELECT /*+ RULE +*/
       ora_database_name    AS primary_catalog_name
      ,pri.owner            AS primary_schema_name
      ,pri.table_name       AS primary_table_name
      ,ora_database_name    AS foreign_catalog_name
      ,con.owner            AS foreign_schema_name
      ,con.table_name       AS table_name
      ,pri.constraint_name  AS primary_key_constraint
      ,con.constraint_name  AS foreign_key_constraint
      ,col.position         AS key_sequence
      ,pky.column_name      AS primary_key_column
      ,col.column_name      AS foreign_key_column
      ,0                    AS update_rule
      ,CASE con.delete_rule WHEN 'RESTRICT'    THEN 1
                            WHEN 'CASCADE'     THEN 0
                            WHEN 'SET NULL'    THEN 2
                            WHEN 'SET DEFAULT' THEN 4
                            WHEN 'NO ACTION'   THEN 3
                            ELSE 0 END AS delete_rule
      ,CASE con.deferrable  WHEN 'NOT DEFERRABLE' THEN 0
                            WHEN 'DEFERRABLE'     THEN 1
                            ELSE 0 END AS DEFERRABLE
      ,0                    AS match_option
      ,CASE con.deferred    WHEN 'IMMEDIATE'      THEN 0
                            ELSE 1 END AS initially_deferred
      ,CASE con.status      WHEN 'ENABLED' THEN 1
                            ELSE 0 END AS enabled
  FROM all_tables tab
       INNER JOIN all_constraints  con ON (con.owner           = tab.owner
                                      AND  con.table_name      = tab.table_name)
       INNER JOIN all_cons_columns col ON (con.owner           = col.owner
                                      AND  con.constraint_name = col.constraint_name)
       INNER JOIN all_constraints  pri ON (pri.owner           = con.r_owner
                                      AND  pri.constraint_name = con.r_constraint_name)
       INNER JOIN all_cons_columns pky ON (pri.owner           = pky.owner
                                      AND  pri.constraint_name = pky.constraint_name)
 WHERE col.position        = pky.position
   AND con.constraint_type = 'R'
   AND tab.owner           = 'MDBA'
   AND tab.table_name      = 'KBS_EI'
 ORDER BY 1,2,3,4,5,6,7,8,9
;

SELECT con.*
  FROM all_tables tab
       INNER JOIN all_indexes idx     ON (idx.table_owner = tab.owner AND idx.table_name = tab.table_name)
       INNER JOIN all_constraints con ON (con.index_owner = idx.owner AND con.index_name = idx.index_name)
 WHERE tab.owner      = 'MDBA'
   AND tab.table_name = 'KBS_EI'
;

SELECT con.*
  FROM all_tables tab
       INNER JOIN all_constraints  con ON (con.owner = tab.owner AND con.table_name = tab.table_name)
       INNER JOIN all_cons_columns col ON (con.owner           = col.owner
                                      AND  con.constraint_name = col.constraint_name)
 WHERE tab.owner      = 'MDBA'
   AND tab.table_name = 'KBS_EI'
   AND con.constraint_type IN ('P','R')
;



SELECT *
  FROM all_views -- KU$_CONSTRAINT_EXISTS_VIEW;
 WHERE view_name LIKE '%CONS%';

SELECT * FROM REF_CONSTRAINT_EXISTS_VIEW;