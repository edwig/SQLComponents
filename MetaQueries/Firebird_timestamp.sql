-- Test table 'alldays' with 1 record

CREATE TABLE alldays
(
    id INTEGER
   ,themoment timestamp
);


INSERT INTO alldays VALUES (1,{fn NOW()});

SELECT * FROM alldays;