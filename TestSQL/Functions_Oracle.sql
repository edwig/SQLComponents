DROP TABLE testrecord;

CREATE TABLE testrecord
(
   name VARCHAR2(4000)
);
/

CREATE OR REPLACE PROCEDURE testmul(p_num INTEGER)
RETURN INTEGER AS
BEGIN
  RETURN p_num * 2;
END testmul;
/


CREATE OR REPLACE FUNCTION testins(p_text IN VARCHAR2)
RETURN NUMBER as
BEGIN
  RETURN Length(p_text);
END testins;
/

CREATE OR REPLACE FUNCTION multinout(p_text IN VARCHAR2,p_total IN OUT VARCHAR)
RETURN NUMBER IS
  v_plus VARCHAR2(4000);
BEGIN
  v_plus := p_text || p_text;
  INSERT INTO testrecord (name) VALUES (v_plus);
  INSERT INTO testrecord (name) VALUES (p_total);
  COMMIT;
  p_total := SubStr(v_plus,1,200);
  RETURN 88.77;
END multinout;
/

SELECT testins('wocas') FROM dual;

SELECT * FROM testrecord;
DELETE FROM testrecord;


SELECT testins(p_text => 'wocas') FROM dual;