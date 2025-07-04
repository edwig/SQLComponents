-- GETTING THE PROCEDURE TEXT
-- FIREBIRD 4 SQL
--
-- COMBINED

SELECT 'CREATE OR ALTER PROCEDURE "' || Trim(rdb$procedure_name) || '"' AS text
      ,-1 AS pos
  FROM rdb$procedures
 WHERE rdb$procedure_name = 'Sales by Year'

UNION

SELECT CASE WHEN rdb$parameter_number = min_number THEN '(' ELSE ' ' END ||
       '"' || Trim(rdb$parameter_name) || '" ' ||
       CASE Trim(typ.rdb$type_name)
           WHEN 'SHORT'   THEN 'NUMERIC'        -- extra
           WHEN 'LONG'    THEN 'NUMERIC'        -- extra
           WHEN 'INT64'   THEN 'NUMERIC'
		   WHEN 'TEXT'    THEN 'CHAR'			-- extra
           WHEN 'VARYING' THEN 'VARCHAR'
           ELSE Trim(typ.rdb$type_name)
       END ||
       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0
                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||
       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0
                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END
            ELSE '' END ||
       CASE WHEN rdb$parameter_number = max_number THEN ')' ELSE ',' END AS param_name
      ,rdb$parameter_number AS pos
  FROM (SELECT par.rdb$parameter_name
              ,par.rdb$parameter_number
              ,par.rdb$field_source
              ,(SELECT Min(rdb$parameter_number)
                  FROM rdb$procedure_parameters pi
                 WHERE pi.rdb$procedure_name = 'Sales by Year'
                   AND pi.rdb$parameter_type = 0) AS min_number
              ,(SELECT Max(rdb$parameter_number)
                  FROM rdb$procedure_parameters pi
                 WHERE pi.rdb$procedure_name = 'Sales by Year'
                   AND pi.rdb$parameter_type = 0) AS max_number
          FROM rdb$procedure_parameters par
         WHERE rdb$procedure_name = 'Sales by Year'
           AND rdb$parameter_type = 0
         ORDER BY par.rdb$parameter_number)
       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source
       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type
                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')

UNION


SELECT Trim(CASE WHEN rdb$parameter_number = min_number THEN 'RETURNS (' || ASCII_CHAR(10) ELSE '' END) ||
       '"' || Trim(rdb$parameter_name) || '" ' ||
       CASE Trim(typ.rdb$type_name)
           WHEN 'SHORT'   THEN 'NUMERIC'        -- extra
           WHEN 'LONG'    THEN 'NUMERIC'        -- extra
           WHEN 'INT64'   THEN 'NUMERIC'
		   WHEN 'TEXT'    THEN 'CHAR'			-- extra
           WHEN 'VARYING' THEN 'VARCHAR'
           ELSE Trim(typ.rdb$type_name)
       END ||
       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0
                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||
       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0
                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END
            ELSE '' END ||
       CASE WHEN rdb$parameter_number = max_number THEN ')' ELSE ',' END AS param_name
      ,rdb$parameter_number + 5000 AS pos
  FROM (SELECT par.rdb$parameter_name
              ,par.rdb$parameter_number
              ,par.rdb$field_source
              ,(SELECT Min(rdb$parameter_number)
                  FROM rdb$procedure_parameters pi
                 WHERE pi.rdb$procedure_name = 'Sales by Year'
                   AND pi.rdb$parameter_type = 1) AS min_number
              ,(SELECT Max(rdb$parameter_number)
                  FROM rdb$procedure_parameters pi
                 WHERE pi.rdb$procedure_name = 'Sales by Year'
                   AND pi.rdb$parameter_type = 1) AS max_number
          FROM rdb$procedure_parameters par
         WHERE rdb$procedure_name = 'Sales by Year'
           AND rdb$parameter_type = 1
          ORDER BY par.rdb$parameter_number)
       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source
       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type
                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')
UNION

SELECT 'AS' || ASCII_CHAR(10) ||
       rdb$procedure_source AS text
      ,10000 AS pos
  FROM rdb$procedures
 WHERE rdb$procedure_name = 'Sales by Year'

ORDER BY 2

;


-- CHAT GPT 4.0

WITH param_types AS 
(
 SELECT p.RDB$PROCEDURE_NAME,
        p.RDB$PARAMETER_NAME,
        p.RDB$PARAMETER_TYPE, -- 0 = input, 1 = output
        f.RDB$FIELD_TYPE,
        f.RDB$FIELD_SUB_TYPE,
        f.RDB$FIELD_LENGTH,
        f.RDB$FIELD_PRECISION,
        f.RDB$FIELD_SCALE,
        f.RDB$CHARACTER_LENGTH,
        f.RDB$FIELD_NAME AS FIELD_SOURCE,
        t.RDB$TYPE_NAME AS FIELD_TYPE_NAME
   FROM RDB$PROCEDURE_PARAMETERS p
        JOIN RDB$FIELDS f ON p.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME
        JOIN RDB$TYPES  t ON f.RDB$FIELD_TYPE   = t.RDB$TYPE AND t.RDB$FIELD_NAME = 'RDB$FIELD_TYPE'
),
params AS 
(
 SELECT RDB$PROCEDURE_NAME,
        RDB$PARAMETER_TYPE,
        TRIM(RDB$PARAMETER_NAME) AS PARAM_NAME,
        TRIM(CASE
            WHEN FIELD_TYPE_NAME = 'VARYING' THEN 'VARCHAR(' || f.RDB$CHARACTER_LENGTH || ')'
            WHEN FIELD_TYPE_NAME = 'TEXT' THEN 'CHAR(' || f.RDB$CHARACTER_LENGTH || ')'
            WHEN FIELD_TYPE_NAME IN ('SHORT', 'LONG', 'INT64') AND f.RDB$FIELD_SCALE < 0 THEN
                'NUMERIC(' || f.RDB$FIELD_PRECISION || ', ' || ABS(f.RDB$FIELD_SCALE) || ')'
            ELSE FIELD_TYPE_NAME
        END) AS PARAM_TYPE
   FROM param_types f
),
input_params AS 
(
 SELECT RDB$PROCEDURE_NAME, LIST(PARAM_NAME || ' ' || PARAM_TYPE) AS PARAM_LIST
   FROM params
  WHERE RDB$PARAMETER_TYPE = 0
  GROUP BY RDB$PROCEDURE_NAME
),
output_params AS 
(
 SELECT RDB$PROCEDURE_NAME, LIST(PARAM_NAME || ' ' || PARAM_TYPE) AS RETURN_LIST
   FROM params
  WHERE RDB$PARAMETER_TYPE = 1
  GROUP BY RDB$PROCEDURE_NAME
)
SELECT 'CREATE OR ALTER PROCEDURE ' || TRIM(p.RDB$PROCEDURE_NAME) || 
       COALESCE(        ' (' || i.PARAM_LIST  || ')', '') ||
       COALESCE(' RETURNS (' || o.RETURN_LIST || ')', '') || 
       ' AS ' || 
       COALESCE(p.RDB$PROCEDURE_SOURCE,'') AS PROCEDURE_DEFINITION
  FROM RDB$PROCEDURES p
       LEFT JOIN input_params  i ON p.RDB$PROCEDURE_NAME = i.RDB$PROCEDURE_NAME
       LEFT JOIN output_params o ON p.RDB$PROCEDURE_NAME = o.RDB$PROCEDURE_NAME
 ORDER BY p.RDB$PROCEDURE_NAME;


