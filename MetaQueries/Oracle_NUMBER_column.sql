-- Changing would-be-integers back to NUMBER
-- Needed for conversion to other database types

SELECT 'alter table ' || col.table_name || ' modify ' || column_name || ' number;'
  FROM all_tab_columns col
       INNER JOIN all_tables tab ON (col.owner = tab.owner AND col.table_Name = tab.table_Name)
 WHERE col.owner       = 'MDBA'
   AND col.data_type   = 'NUMBER'
   AND col.data_scale  = 0;