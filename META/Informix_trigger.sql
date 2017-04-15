CREATE PROCEDURE detail_amount(p_id INTEGER,p_value DECIMAL(18,2))

UPDATE detail
   SET amount = p_value
 WHERE id = p_id;

END PROCEDURE;




CREATE TRIGGER detail_check UPDATE ON detail
REFERENCING OLD AS o
            NEW AS n
FOR EACH ROW
(
  EXECUTE PROCEDURE detail_amount(n.id,n.amount)
);




SELECT * FROM systriggers
SELECT * FROM systrigbody;

SELECT datakey,seqno,data
  FROM systrigbody
 WHERE datakey IN ('A','D')
 ORDER BY datakey DESC,seqno;