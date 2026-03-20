-- EXISTS

SELECT trim(dbs.mon$database_name) as catalog_name
      ,rdb$owner_name
      ,rdb$package_name
  FROM rdb$procedures
       CROSS JOIN mon$database dbs
 WHERE rdb$private_flag >= 0
UNION
SELECT trim(dbs.mon$database_name)
      ,rdb$owner_name
      ,rdb$package_name
  FROM rdb$functions
       CROSS JOIN mon$database dbs
 WHERE rdb$private_flag >= 0
;

-- MODULES


SELECT rdb$owner_name
      ,rdb$package_name
      ,rdb$procedure_name
      ,rdb$private_flag
      ,'PROCEDURE' AS type
  FROM rdb$procedures
 WHERE rdb$private_flag >= 0

UNION ALL

SELECT rdb$owner_name
      ,rdb$package_name
      ,rdb$function_name
      ,rdb$private_flag
      ,'FUNCTION'
  FROM rdb$functions
 WHERE rdb$private_flag >= 0
;



