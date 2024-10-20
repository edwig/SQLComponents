SELECT sys_context('USERENV','DB_NAME') AS procedure_catalog
      ,owner
      ,object_name
      ,1 AS procedure_type
 FROM all_procedures
WHERE object_type = 'PROCEDURE'
  AND owner = 'VIEWPOINT'
UNION
SELECT sys_context('USERENV','DB_NAME')
      ,owner
      ,object_name || '.' || procedure_name
      ,1
 FROM all_procedures
WHERE object_type = 'PACKAGE'
  AND procedure_name IS NOT null
  AND owner = 'VIEWPOINT'
UNION
SELECT sys_context('USERENV','DB_NAME')
      ,owner
      ,object_name
      ,2
 FROM all_procedures
WHERE object_type = 'FUNCTION'
  AND owner = 'VIEWPOINT'
ORDER BY 1,2,3


;
-- PROCEDURE/FUNCTION + ARGUMENTS

-- PROCEDURE WITHOUT PACKAGE
SELECT
    p.owner,
    p.object_name,
    'PROCEDURE' AS object_type,
    a.argument_name,
    a.position,
    a.data_type,
    a.in_out
FROM
    all_procedures p
JOIN
    all_arguments a
ON
    p.object_name = a.object_name
AND p.object_id   = a.object_id
WHERE
    p.object_type IN ('PACKAGE','PROCEDURE')
AND p.owner       = 'VIEWPOINT'
AND p.object_name = 'JAVA_RED_CMD'
UNION
-- PROCEDURE/FUNCTION FROM PACKAGE
SELECT
    p.owner,
    p.object_name || '.' || p.procedure_name AS procedure_name,
    'PROCEDURE' AS object_type,
    a.argument_name,
    a.position,
    a.data_type,
    a.in_out
FROM
    all_procedures p
JOIN
    all_arguments a
ON
    p.object_name    = a.package_name
AND p.procedure_name = a.object_name
AND p.object_id      = a.object_id
WHERE
    p.object_type IN 'PACKAGE'
AND p.owner       = 'VIEWPOINT'
AND p.procedure_name = 'JAVA_RED_CMD'
UNION
-- FUNCTION WITHOUT PACKAGE
SELECT
    p.owner,
    p.object_name AS procedure_name,
    p.object_type,
    a.argument_name,
    a.position,
    a.data_type,
    a.in_out
FROM
    all_procedures p
JOIN
    all_arguments a
ON
    a.package_name IS NULL
AND p.object_name = a.object_name
AND p.object_id      = a.object_id
WHERE
    p.object_type = 'FUNCTION'
AND p.owner       = 'VIEWPOINT'
AND p.object_name = 'JAVA_RED_CMD'
ORDER BY 1,2,3,5,4

