-- MS Types

CREATE TYPE northwind.name_identifier FROM VARCHAR(128) NOT NULL;
CREATE TYPE northwind.optional_name   FROM VARCHAR(128);


CREATE TYPE northwind.order_lines
AS TABLE
(
  order_id INT
 ,item_id  INT
 ,quantity decimal(10,2)
 ,info     VARCHAR(1000)
 ,expected date
);

EXEC sp_addextendedproperty
    @name   = N'MS_Description',
    @value = N'Standard ISO name identifier for SQL',
    @level0type = N'SCHEMA', @level0name = 'northwind',
    @level1type = N'TYPE',   @level1name = 'name_identifier'
;

EXEC sp_addextendedproperty
    @name   = N'MS_Description',
    @value = N'Set with all order lines to proces',
    @level0type = N'SCHEMA', @level0name = 'northwind',
    @level1type = N'TYPE',   @level1name = 'order_lines'
;

EXEC sp_addextendedproperty
    @name  = N'MS_Description',
    @value = N'Total shipped for this item',
    @level0type = N'SCHEMA', @level0name = 'northwind',
    @level1type = N'TYPE',   @level1name = 'order_lines',
    @level2type = N'COLUMN', @level2name = 'quantity'
;


-- CHAT-GPT: Base types

SELECT SCHEMA_NAME(t.schema_id) AS [Schema],
       t.name AS [TypeName],
       t.is_table_type,
       t.is_user_defined,
       bt.name AS [BaseType],
       t.max_length,
       t.precision,
       t.scale,
       t.is_nullable
  FROM sys.types t
       LEFT JOIN sys.types bt ON t.system_type_id = bt.user_type_id AND bt.is_user_defined = 0
WHERE t.is_user_defined = 1
  AND t.is_table_type   = 0;

-- CHAT-GPT: Table types

SELECT tt.name AS [TypeName],
       c.name AS [ColumnName],
       ty.name AS [DataType],
       c.max_length,
       c.precision,
       c.scale,
       c.is_nullable
 FROM sys.table_types tt
      JOIN sys.columns c ON tt.type_table_object_id = c.object_id
      JOIN sys.types  ty ON c.user_type_id = ty.user_type_id
WHERE tt.is_user_defined = 1;

----------------------------------
-- LIST

SELECT db_name() AS catalogname
      ,s.name    AS schemaname
      ,t.name    AS typename
      ,CASE t.is_table_type
            WHEN 0 THEN 'D'
            WHEN 1 THEN 'C'
            ELSE ''
       END       AS typeusage
      ,1         AS ordinal
      ,''        AS attribute
      ,bt.name   AS datatype
      ,CASE WHEN t.is_nullable = 1 THEN 0 ELSE 1 END AS notnullable
      ,''        AS defaultvalue
      ,''        AS domaincheck
      ,CASE e.name
            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))
            ELSE ''
       END  AS remarks
      ,'<@>'     AS source
  FROM sys.types t
       JOIN sys.schemas s ON s.schema_id = t.schema_id
       LEFT OUTER JOIN sys.types  bt ON t.system_type_id = bt.user_type_id
                                    AND bt.is_user_defined = 0
       LEFT OUTER JOIN sys.extended_properties e ON
                     ((e.major_id  = t.user_type_id OR e.major_id IS null)
                  AND e.minor_id  = 0)
 WHERE t.is_user_defined = 1
   AND s.name = 'northwind';



----------------------------------
-- GET THE ATTRIBUTES

SELECT db_name() AS catalogname
      ,s.name    AS schemaname
      ,t.name    AS typename
      ,'D'       AS typeusage
      ,1         AS ordinal
      ,''        AS attribute
      ,bt.name +
       CASE
          WHEN bt.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')
               THEN '(' + CASE WHEN t.max_length = -1 THEN 'MAX' ELSE CAST(t.max_length AS VARCHAR) END + ')'
          WHEN bt.name IN ('decimal', 'numeric')
               THEN '(' + CAST(t.precision AS VARCHAR) + ',' + CAST(t.scale AS VARCHAR) + ')'
          ELSE ''
       END AS datatype
      ,CASE WHEN t.is_nullable = 1 THEN 0 ELSE 1 END AS notnullable
      ,''        AS defaultvalue
      ,''        AS domaincheck
      ,CASE e.name
            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))
            ELSE ''
       END  AS remarks
      ,'<@>'     AS source
  FROM sys.types t
       JOIN sys.schemas s ON s.schema_id = t.schema_id
       JOIN sys.types  bt ON t.system_type_id = bt.user_type_id AND bt.is_user_defined = 0
       LEFT OUTER JOIN sys.extended_properties e ON
                     ((e.major_id  = t.user_type_id OR e.major_id IS null)
                  AND e.minor_id  = 0)
 WHERE t.is_user_defined = 1
   AND t.is_table_type   = 0
   AND s.name = 'northwind';





SELECT db_name()   AS catalogname
      ,s.name      AS schemaname
      ,tt.name     AS typename
      ,'C'         AS typeusage
      ,c.column_id AS ordinal
      ,c.name      AS attribute
      ,ty.name +
       CASE
           WHEN ty.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')
                THEN '(' + CASE WHEN c.max_length = -1 THEN 'MAX' ELSE CAST(c.max_length AS VARCHAR) END + ')'
           WHEN ty.name IN ('decimal', 'numeric')
                THEN '(' + CAST(c.precision AS VARCHAR) + ',' + CAST(c.scale AS VARCHAR) + ')'
           ELSE ''
       END AS datatype
      ,CASE WHEN c.is_nullable = 0 THEN 1 ELSE 0 END AS notnull
      ,''         AS defaultvalue
      ,''         AS domaincheck
      ,CASE e.name
            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))
            ELSE ''
       END  AS remarks
      ,'<@>'      AS source
  FROM sys.table_types tt
       JOIN sys.schemas s ON s.schema_id = tt.schema_id
       JOIN sys.columns c ON tt.type_table_object_id = c.object_id
       JOIN sys.types  ty ON c.user_type_id = ty.user_type_id
       LEFT OUTER JOIN sys.extended_properties e ON
                     ((e.major_id  = tt.user_type_id OR e.major_id IS null)
                   AND e.minor_id  = c.column_id)
 WHERE tt.is_user_defined = 1
   AND  s.name = 'northwind'
   AND tt.name = 'order_lines'
 ORDER BY 1,2,3,5;


-- GET THE SOURCE


SELECT t.name
      ,1 AS line
      ,'CREATE TYPE [' + s.name + '].[' + t.name + '] FROM ' +
       bt.name +
       CASE
          WHEN bt.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')
               THEN '(' + CASE WHEN t.max_length = -1 THEN 'MAX' ELSE CAST(t.max_length AS VARCHAR) END + ')'
          WHEN bt.name IN ('decimal', 'numeric')
               THEN '(' + CAST(t.precision AS VARCHAR) + ',' + CAST(t.scale AS VARCHAR) + ')'
          ELSE ''
       END +
       CASE WHEN t.is_nullable = 0 THEN ' NOT NULL' ELSE ' NULL' END AS [CreateTypeStatement]
  FROM sys.types t
       JOIN sys.schemas s ON s.schema_id = t.schema_id
       JOIN sys.types  bt ON t.system_type_id = bt.user_type_id AND bt.is_user_defined = 0
 WHERE t.is_user_defined = 1
   AND t.is_table_type   = 0
   AND s.name = 'northwind';


SELECT tt.name
      ,1 AS line
      ,'CREATE TYPE [' + s.name + '].[' + tt.name + '] AS TABLE (' + CHAR(13) +
       STRING_AGG(
           '    [' + c.name + '] ' +
           ty.name +
           CASE
               WHEN ty.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')
                   THEN '(' + CASE WHEN c.max_length = -1 THEN 'MAX' ELSE CAST(c.max_length AS VARCHAR) END + ')'
               WHEN ty.name IN ('decimal', 'numeric')
                   THEN '(' + CAST(c.precision AS VARCHAR) + ',' + CAST(c.scale AS VARCHAR) + ')'
               ELSE ''
           END +
           CASE WHEN c.is_nullable = 0 THEN ' NOT NULL' ELSE ' NULL' END,
           ',' + CHAR(13)
       ) + CHAR(13) + ')' AS [CreateTableTypeStatement]
  FROM sys.table_types tt
       JOIN sys.schemas s ON s.schema_id = tt.schema_id
       JOIN sys.columns c ON tt.type_table_object_id = c.object_id
       JOIN sys.types ty ON c.user_type_id = ty.user_type_id
 WHERE tt.is_user_defined = 1
   AND  s.name = 'northwind'
   AND tt.name = 'order_lines'
 GROUP BY s.name
         ,tt.name;








SELECT * FROM sys.extended_properties;
