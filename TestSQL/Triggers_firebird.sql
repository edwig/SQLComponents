-- Triggers
SELECT rdb$trigger_name
      ,rdb$description
      ,rdb$trigger_sequence
      ,CASE rdb$trigger_type
            WHEN 1 THEN 'BEFORE INSERT'
            WHEN 2 THEN 'AFTER INSERT'
            WHEN 3 THEN 'BEFORE UPDATE'
            WHEN 4 THEN 'AFTER UPDATE'
            WHEN 5 THEN 'BEFORE DELETE'
            WHEN 6 THEN 'AFTER DELETE'
            WHEN 17 THEN 'BEFORE INSERT OR UPDATE'
            WHEN 18 THEN 'AFTER INSERT OR UPDATE'
            WHEN 25 THEN 'BEFORE INSERT OR DELETE'
            WHEN 26 THEN 'AFTER INSERT OR DELETE'
            WHEN 27 THEN 'BEFORE UPDATE OR DELETE'
            WHEN 28 THEN 'AFTER UPDATE OR DELETE'
            WHEN 113 THEN 'BEFORE INSERT OR UPDATE OR DELETE'
            WHEN 114 THEN 'AFTER INSERT OR UPDATE OR DELETE'
            WHEN 8192 THEN 'ON CONNECT'
            WHEN 8193 THEN 'ON DISCONNECT'
            WHEN 8194 THEN 'ON TRANSACTION START'
            WHEN 8195 THEN 'ON TRANSACTION COMMIT'
            WHEN 8196 THEN 'ON TRANSACTION ROLLBACK'
            ELSE 'UNKNOWN'
       END  AS trigger_when
      ,rdb$trigger_source
  FROM rdb$triggers
 WHERE rdb$relation_name = 'ALL_TYPES'
   AND rdb$system_flag   = 0
 ORDER BY rdb$trigger_sequence;


113 - before insert or update or delete
114 - after insert or update or delete
8192 - on connect
8193 - on disconnect
8194 - on transaction start
8195 - on transaction commit
8196 - on transaction rollback



AS
BEGIN
  NEW.col_three = new.col_two * 1.20;
END



