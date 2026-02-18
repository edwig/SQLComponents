ALTER TABLE kiw_bat
  ADD CONSTRAINT  kiw_bat_14986
      FOREIGN KEY (bat_oid)
      REFERENCES  kiw_tvs(oid)

ALTER TABLE kiw_bat
 ALTER COLUMN bat_oid TYPE INTEGER;



ALTER TABLE kiw_bat
ADD bat_oid_xx INTEGER;

UPDATE kiw_bat
SET bat_oid_xx = bat_oid;

DROP VIEW batterij;
DROP VIEW batterij_h;
DROP VIEW batterij_n;
DROP VIEW batterij_g;
DROP VIEW kiw_bat_h;
DROP VIEW kiw_bat_n;
DROP VIEW kiw_bat_g;

DROP PROCEDURE kiw_bat_ins_proc;
DROP PROCEDURE kiw_bat_upd_proc;
DROP PROCEDURE kiw_bat_del_proc;
DROP PROCEDURE kiw_bat_fupd_proc;
DROP PROCEDURE kiw_tvs_del_proc;

ALTER TABLE kiw_bat
DROP bat_oid;

ALTER TABLE kiw_bat
ALTER COLUMN bat_oid_xx TO bat_oid;

SELECT rdb$dependent_name
      ,CASE rdb$dependent_type
            WHEN 0  THEN 'TABLE'
            WHEN 1  THEN 'VIEW'
            WHEN 2  THEN 'TRIGGER'
            WHEN 3  THEN 'COMPUTED BY'
            WHEN 4  THEN 'CHECK'
            WHEN 5  THEN 'PROCEDURE'
            WHEN 6  THEN 'INDEX'
            WHEN 7  THEN 'EXCEPTION'
            WHEN 8  THEN 'USER'
            WHEN 9  THEN 'COLUMN'
            WHEN 10 THEN 'INDEX'
            WHEN 15 THEN 'FUNCTION'
            WHEN 18 THEN 'PACKAGE HEADER'
            WHEN 19 THEN 'PACKAGE BODY'
                    ELSE 'UNKNOWN'
       END AS type
  FROM rdb$dependencies
 WHERE rdb$depended_on_name = 'KIW_BAT'
   AND rdb$field_name       = 'BAT_OID';

SELECT * FROM rdb$dependencies;
