-- COMPOSITE TYPE ATTRIBUTES LIST

SELECT current_catalog as catalogname
      ,n.nspname    as schemaname
	  ,t.typname    as typename
	  ,'C'          as typeusage
	  ,a.attnum     as ordinal
	  ,a.attname    as attribute
	  ,pg_catalog.format_type(a.atttypid, a.atttypmod) as datatype
	  ,t.typenotnull as notnull
	  ,''           as defaultvalue
	  ,''           as domaincheck
	  ,''           as remarks
	  ,'<@>'        as source
  FROM pg_type t
       JOIN pg_class     c ON c.oid = t.typrelid
       JOIN pg_namespace n ON n.oid = c.relnamespace
       JOIN pg_attribute a ON c.oid = a.attrelid
 WHERE t.typtype     = 'c'
   and c.relkind     = 'c'
   AND a.attnum      > 0 
   AND NOT a.attisdropped
   AND n.nspname NOT IN ('pg_catalog', 'information_schema')
   and n.nspname = 'northwind'
 ORDER by 1,2,3,5;

-- ENUM TYPES ATTRIBUTES LIST

SELECT current_catalog as catalogname
      ,n.nspname       as schemaname
   	  ,t.typname       as typename
      ,'E'             as typeusage
      ,e.enumsortorder as ordinal
      ,e.enumlabel     as attribute
      ,''              as datatype
      ,false           as notnull
	  ,''              as defaultvalue
      ,''              as domaincheck
      ,''              as remarks
      ,'<@>'           as source
  FROM pg_type t
       JOIN pg_enum      e ON t.oid = e.enumtypid
       JOIN pg_namespace n ON n.oid = t.typnamespace
 WHERE t.typtype     = 'e'
   AND n.nspname NOT IN ('pg_catalog', 'information_schema')
   AND n.nspname = 'northwind'
   AND t.typname = 'Province'
 ORDER by 1,2,3,5;

SELECT current_catalog as catalogname
      ,n.nspname       as schemaname
   	  ,t.typname       as typename
      ,'D'             as typeusage
      ,1               as ordinal
      ,''              as attribute
      ,pg_catalog.format_type(t.typbasetype, t.typtypmod) as datatype
      ,t.typnotnull    as notnull
      ,t.typdefault    AS defaultvalue
      ,cc.checks       as domaincheck
      ,''              as remarks
      ,'<@>'           as source
  FROM pg_catalog.pg_type t
       INNER JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace
       LEFT OUTER JOIN (
                  SELECT r.contypid, string_agg(pg_get_constraintdef(r.oid, true), ' AND ') AS checks
                    FROM pg_catalog.pg_constraint r
                   WHERE r.contype = 'c'
                   GROUP BY r.contypid
                 ) cc ON cc.contypid = t.oid
 WHERE t.typtype = 'd'
   AND n.nspname NOT IN ('pg_catalog', 'information_schema')
   AND n.nspname = 'northwind'
   AND t.typname = 'Rabat'
 ORDER by 1,2,3,5;


-- SELECT SOURCE CODE

SELECT 'CREATE TYPE ' || n.nspname || '.' || t.typname || ' AS (' ||
       string_agg(a.attname || ' ' || pg_catalog.format_type(a.atttypid, a.atttypmod), ', ') ||
       ');' AS create_composite_statement
  FROM pg_type t
       JOIN pg_class c ON t.typrelid = c.oid
       JOIN pg_namespace n ON c.relnamespace = n.oid
       JOIN pg_attribute a ON a.attrelid = c.oid
 WHERE t.typtype = 'c'
   AND c.relkind = 'c'
   AND a.attnum  > 0 
   AND NOT a.attisdropped
   AND n.nspname NOT IN ('pg_catalog', 'information_schema')
 GROUP BY n.nspname, t.typname;

SELECT 'CREATE TYPE ' || n.nspname || '.' || t.typname || ' AS ENUM (' ||
       string_agg(quote_literal(e.enumlabel), ', ') || ');' AS create_enum_statement
  FROM pg_type t
       JOIN pg_enum e ON t.oid = e.enumtypid
       JOIN pg_namespace n ON t.typnamespace = n.oid
 WHERE t.typtype = 'e'
   AND n.nspname NOT IN ('pg_catalog', 'information_schema')
GROUP BY n.nspname, t.typname;


SELECT 'CREATE DOMAIN ' || n.nspname || '.' || t.typname || 
       ' AS ' || pg_catalog.format_type(t.typbasetype, t.typtypmod) ||
       CASE WHEN t.typnotnull THEN ' NOT NULL' ELSE '' END ||
       CASE WHEN t.typdefault IS NOT NULL THEN ' DEFAULT ' || t.typdefault ELSE '' END ||
       CASE 
          WHEN cc.checks IS NOT NULL THEN ' ' || cc.checks
          ELSE '' 
       END ||
       ';' AS create_domain_statement
  FROM pg_catalog.pg_type t
       INNER JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace
       LEFT OUTER JOIN (
                  SELECT r.contypid, string_agg(pg_get_constraintdef(r.oid, true), ' AND ') AS checks
                    FROM pg_catalog.pg_constraint r
                   WHERE r.contype = 'c'
                   GROUP BY r.contypid
                 ) cc ON cc.contypid = t.oid
  WHERE t.typtype = 'd'
    AND n.nspname NOT IN ('pg_catalog', 'information_schema')
 ORDER BY n.nspname, t.typname;
