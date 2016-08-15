-- TEST SCRIPT MasterDetail.sql
-- For the SQLComponents test suite

-- DROP TABLE master;
-- DROP TABLE detail;

CREATE TABLE master
(
   id           INTEGER
  ,invoice      INTEGER
  ,description  VARCHAR2(250)
  ,total        NUMBER
);

ALTER TABLE master
  ADD CONSTRAINT pk_master PRIMARY KEY (id);


CREATE TABLE detail
(
   id          INTEGER
  ,mast_id     INTEGER
  ,line        INTEGER
  ,description VARCHAR2(250)
  ,amount      number
);

ALTER TABLE detail
  ADD CONSTRAINT pk_detail PRIMARY KEY(id);

ALTER TABLE detail
  ADD CONSTRAINT fk_detail_master
      FOREIGN KEY (mast_id)
      REFERENCES master(id);


-- First invoice (total amount is incorrect!)
INSERT INTO master (id, invoice, description, total)
VALUES (1,1001,'First invoice',300);

INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (1,1,1,'First line of first invoice',100);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (2,1,2,'Second line of first invoice',500);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (3,1,3,'Third line of first invoice',150);

-- Second invoice
INSERT INTO master (id, invoice, description, total)
VALUES (2,1002,'Second invoice',600);

INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (4,2,1,'First line of second invoice',50);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (5,2,2,'Second line of second invoice',50);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (6,2,3,'Third line of second invoice',50);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (7,2,4,'Fourth line of second invoice',50);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (8,2,5,'Fifth line of second invoice',300);
INSERT INTO detail(id, mast_id,line,description,amount)
VALUES (9,2,6,'VAT for the second invoice',100);


SELECT * FROM master;
SELECT * FROM detail;