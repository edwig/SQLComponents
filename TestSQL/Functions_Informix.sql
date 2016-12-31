-- INFORMIX TESTING

CREATE TABLE dual
(
   x VARCHAR(1)
);

INSERT INTO dual VALUES ('X');

SELECT * FROM dual;


DROP FUNCTION testmul2;

CREATE FUNCTION testmul2(p_x INT) RETURNING INT;
  DEFINE num INT;
  SELECT 2 * p_x INTO num FROM dual;
  RETURN num;
END FUNCTION;

SELECT testmul2(12) FROM dual;


CREATE FUNCTION getdecimal(p_x VARCHAR(200)) returning DECIMAL(18,2);
   DEFINE num DECIMAL(18,2);
   LET num = p_x;
   RETURN num;
END FUNCTION;




CREATE TABLE test_record
(
   name VARCHAR(200)
);


CREATE FUNCTION testins(p_x VARCHAR(200)) returning INT;
  INSERT INTO test_record(name) VALUES (p_x);
  RETURN 1;
END FUNCTION;

SELECT * FROM test_record;



DROP PROCEDURE informix.multinout;

CREATE FUNCTION informix.multinout(p_string VARCHAR(200),OUT p_text VARCHAR(200))
   RETURNS DECIMAL(18,2);
   DEFINE extra VARCHAR(200);
   BEGIN
     LET extra = p_string || p_string;
     INSERT INTO test_record(name) VALUES(extra);
   END
   LET p_text = substr(extra,1,100);
   RETURN 77.88;
END FUNCTION;


SELECT * FROM test_record;
DELETE   FROM test_record;

