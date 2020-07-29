-- ORACLE  INDECES META QUERY

SELECT sys_context('USERENV','DB_NAME') AS table_catalog
      ,tab.owner                        AS table_schema
      ,tab.table_name                   AS table_name
      ,NULL          AS non_unique
      ,NULL          AS index_name
      ,0             AS index_type  -- SQL_TABLE_STAT
      ,NULL          AS ordinal_position
      ,NULL          AS column_name
      ,NULL          AS asc_or_desc
      ,tab.num_rows  AS cardinality
      ,tab.blocks    AS pages
      ,NULL          AS filter
  FROM all_tables tab
 WHERE owner      = 'KDBA'
   AND table_name = 'KIW_ENH'

UNION
/


SELECT sys_context('USERENV','DB_NAME') AS table_catalog
      ,ind.table_owner                  AS table_schema
      ,ind.table_name                   AS table_name
      ,CASE ind.uniqueness
            WHEN 'UNIQUE' THEN 0
                          ELSE 1
       END                     AS non_unique
      ,ind.index_name          AS index_name
      ,1                       AS index_type   -- SQL_INDEX_BTREE
      ,col.column_position     AS ordinal_position
      ,col.column_name         AS column_name
      ,substr(col.descend,1,1) AS asc_or_desc
      ,ind.num_rows            AS cardinality
      ,ind.leaf_blocks         AS pages
      ,exp.column_expression   AS filter
  FROM all_indexes ind
       inner join
       all_ind_columns col ON (col.table_name = ind.table_name
                           AND ind.index_name = col.index_name)
       left OUTER JOIN
       all_ind_expressions Exp ON (Exp.table_name      = col.table_name
                               AND Exp.index_name      = col.index_name
                               AND Exp.column_position = col.column_position)
 WHERE ind.table_owner = 'KDBA'
   AND ind.table_name  = 'KIW_ENH'

 ;



