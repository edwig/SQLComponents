select cast('' as varchar(31))     as index_catalog
      ,trim(tab.rdb$owner_name)    as index_schema
      ,trim(idx.rdb$relation_name) as index_table
      ,idx.rdb$unique_flag         as index_unique
      ,trim(idx.rdb$index_name)    as index_name
      ,1                           as index_type
      ,col.rdb$field_position + 1  as index_position
      ,trim(col.rdb$field_name)    as column_name
      ,case idx.rdb$index_type
            when 0 then 'A'
            when 1 then 'D' 
                   else 'A'
       end                         as index_ascending 
      ,col.rdb$statistics          as index_pages
      ,cast('' as varchar(200))    as index_filter
  from rdb$indices        idx
      ,rdb$index_segments col
      ,rdb$relations      tab
 where idx.rdb$index_name    = col.rdb$index_name
   and idx.rdb$relation_name = tab.rdb$relation_name
   and idx.rdb$system_flag   = 0
   and idx.rdb$relation_name = 'DETAIL'
 order by 6
   