-- GETTING THE FUNCTION SOURCE
-- FIREBIRD 4 SQL


SELECT 'CREATE OR ALTER FUNCTION "' || Trim(rdb$functions.rdb$function_name) || '"' AS text
      ,-1 AS pos
  FROM rdb$functions
 WHERE rdb$function_name = 'GETDECIMAL'

UNION


SELECT CASE WHEN rdb$argument_position = min_number THEN '(' ELSE ' ' END ||
       '"' || Trim(rdb$argument_name) || '" ' ||
       CASE Trim(typ.rdb$type_name)
           WHEN 'LONG'    THEN 'INTEGER'
           WHEN 'INT64'   THEN 'DECIMAL'
           WHEN 'VARYING' THEN 'VARCHAR'
           ELSE Trim(typ.rdb$type_name)
       END ||
       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0
                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||
       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0
                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END
            ELSE '' END ||
       CASE WHEN rdb$argument_position = max_number THEN ')' ELSE ',' END AS param_name
      ,rdb$argument_position AS pos
 FROM (SELECT arg.rdb$argument_name
             ,arg.rdb$argument_position
             ,arg.rdb$field_source
             ,(SELECT Min(rdb$argument_position)
                 FROM rdb$function_arguments ai
                WHERE rdb$function_name = 'GETDECIMAL'
                  AND ai.rdb$argument_position > 0) AS min_number
             ,(SELECT Max(rdb$argument_position)
                 FROM rdb$function_arguments ai
                WHERE rdb$function_name = 'GETDECIMAL'
                  AND ai.rdb$argument_position > 0) AS max_number
        FROM rdb$function_arguments arg
       WHERE rdb$function_name = 'GETDECIMAL'
         AND rdb$argument_position > 0
       ORDER BY rdb$argument_position)
       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source
       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type
                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')

UNION

SELECT 'RETURNS ' ||
       CASE Trim(typ.rdb$type_name)
           WHEN 'LONG'    THEN 'INTEGER'
           WHEN 'INT64'   THEN 'DECIMAL'
           WHEN 'VARYING' THEN 'VARCHAR'
           ELSE Trim(typ.rdb$type_name)
       END ||
       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0
                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||
       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0
                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END
            ELSE '' END  AS text
      ,5000 AS pos
  FROM rdb$function_arguments arg
       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = arg.rdb$field_source
       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type
                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')
 WHERE rdb$function_name = 'GETDECIMAL'
   AND arg.rdb$argument_position = 0

UNION

SELECT 'AS' || ASCII_CHAR(10) || rdb$function_source AS text
      ,10000 AS pos
  FROM rdb$functions
 WHERE rdb$function_name = 'GETDECIMAL'


 ORDER BY 2
;


-- CHAT GPT 4

WITH arg_types AS (
    SELECT
        fa.RDB$FUNCTION_NAME,
        fa.RDB$ARGUMENT_NAME,
        fa.RDB$ARGUMENT_POSITION,
        f.RDB$FIELD_TYPE,
        f.RDB$FIELD_SUB_TYPE,
        f.RDB$FIELD_LENGTH,
        f.RDB$FIELD_PRECISION,
        f.RDB$FIELD_SCALE,
        f.RDB$CHARACTER_LENGTH,
        f.RDB$FIELD_NAME AS FIELD_SOURCE,
        t.RDB$TYPE_NAME AS FIELD_TYPE_NAME
    FROM RDB$FUNCTION_ARGUMENTS fa
    JOIN RDB$FIELDS f ON fa.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME
    JOIN RDB$TYPES t ON f.RDB$FIELD_TYPE = t.RDB$TYPE AND t.RDB$FIELD_NAME = 'RDB$FIELD_TYPE'
),
args AS (
    SELECT
        RDB$FUNCTION_NAME,
        RDB$ARGUMENT_POSITION,
        TRIM(RDB$ARGUMENT_NAME) AS ARG_NAME,
        CASE
            WHEN FIELD_TYPE_NAME = 'VARYING' THEN 'VARCHAR(' || f.RDB$CHARACTER_LENGTH || ')'
            WHEN FIELD_TYPE_NAME = 'TEXT' THEN 'CHAR(' || f.RDB$CHARACTER_LENGTH || ')'
            WHEN FIELD_TYPE_NAME IN ('SHORT', 'LONG', 'INT64') AND f.RDB$FIELD_SCALE < 0 THEN
                'NUMERIC(' || f.RDB$FIELD_PRECISION || ', ' || ABS(f.RDB$FIELD_SCALE) || ')'
            ELSE FIELD_TYPE_NAME
        END AS ARG_TYPE
    FROM arg_types f
),
arg_list AS (
    SELECT RDB$FUNCTION_NAME, LIST(ARG_NAME || ' ' || ARG_TYPE ORDER BY RDB$ARGUMENT_POSITION) AS PARAM_LIST
    FROM args
    GROUP BY RDB$FUNCTION_NAME
),
return_type AS (
    SELECT
        f.RDB$FUNCTION_NAME,
        CASE
            WHEN t.RDB$TYPE_NAME = 'VARYING' THEN 'VARCHAR(' || fld.RDB$CHARACTER_LENGTH || ')'
            WHEN t.RDB$TYPE_NAME = 'TEXT' THEN 'CHAR(' || fld.RDB$CHARACTER_LENGTH || ')'
            WHEN t.RDB$TYPE_NAME IN ('SHORT', 'LONG', 'INT64') AND fld.RDB$FIELD_SCALE < 0 THEN
                'NUMERIC(' || fld.RDB$FIELD_PRECISION || ', ' || ABS(fld.RDB$FIELD_SCALE) || ')'
            ELSE t.RDB$TYPE_NAME
        END AS RETURN_TYPE
    FROM RDB$FUNCTIONS f
    JOIN RDB$FIELDS fld ON f.RDB$RETURN_ARGUMENT = fld.RDB$FIELD_NAME
    JOIN RDB$TYPES t ON fld.RDB$FIELD_TYPE = t.RDB$TYPE AND t.RDB$FIELD_NAME = 'RDB$FIELD_TYPE'
)
SELECT
    'CREATE OR ALTER FUNCTION ' || TRIM(f.RDB$FUNCTION_NAME) ||
    COALESCE(' (' || a.PARAM_LIST || ')', '') ||
    ' RETURNS ' || r.RETURN_TYPE || ' AS ' ||
    f.RDB$FUNCTION_SOURCE AS FUNCTION_DEFINITION
FROM RDB$FUNCTIONS f
LEFT JOIN arg_list a ON f.RDB$FUNCTION_NAME = a.RDB$FUNCTION_NAME
LEFT JOIN return_type r ON f.RDB$FUNCTION_NAME = r.RDB$FUNCTION_NAME
ORDER BY f.RDB$FUNCTION_NAME;
