DROP TABLE all_types;

CREATE TABLE all_types
(
    col_one     INTEGER
   ,col_two     FLOAT
   ,col_three   real
   ,col_four    DECIMAL(18,2)
   ,col_five    CHAR(1)
   ,col_six     VARCHAR(100)
   ,col_seven   SMALLINT
   ,col_eight   BIGINT
   ,col_nine    national CHARACTER varying (100)
   ,col_ten     TIME
   ,col_eleven  DATE
   ,col_twelve  TIMESTAMP

);

CREATE OR ALTER TRIGGER all_types_check FOR all_types
BEFORE INSERT OR UPDATE
AS
BEGIN
  NEW.col_three = new.col_two * 1.20;
END

COMMIT;

INSERT INTO all_types (col_one,col_two)
VALUES (10,10);

UPDATE all_types
   SET col_two = 20;

SELECT * FROM all_types;