-- ORACLE ADT (Abstract Data Type)
--

-- Run as SYSTEM or SYS
GRANT CREATE TYPE TO northwind;
GRANT UNDER ANY TYPE TO northwind;
GRANT SELECT ON V_$SQL_PLAN TO northwind;
GRANT SELECT ON V_$SESSION  TO northwind;
GRANT SELECT ON V_$SQL_PLAN_STATISTICS_ALL  TO northwind;
GRANT SELECT ON ALL_SQLSET_PLANS       TO northwind;
GRANT SELECT ON ALL_SQLSET_STATEMENTS  TO northwind;
GRANT SELECT ON DBA_SQL_PLAN_BASELINES TO northwind;


-- Run as 'northwind' on the 'northwind' schema

CREATE TYPE northwind.cust_address_typ AS VARRAY(5)  OF VARCHAR2(100);
CREATE TYPE northwind.phone_list_typ   AS VARRAY(5)  OF VARCHAR2(25);
CREATE TYPE northwind.order_list_typ   AS VARRAY(20) OF NUMBER(6);

CREATE OR REPLACE TYPE northwind.customer_typ AS OBJECT
(    customer_id        NUMBER(6)
    ,cust_first_name    VARCHAR2(20)
    ,cust_last_name     VARCHAR2(20)
    ,cust_address       CUST_ADDRESS_TYP
    ,phone_numbers      PHONE_LIST_TYP
    ,nls_language       VARCHAR2(3)
    ,nls_territory      VARCHAR2(30)
    ,credit_limit       NUMBER(9,2)
    ,cust_email         VARCHAR2(30)
    ,cust_orders        ORDER_LIST_TYP
);

CREATE TABLE northwind.my_customers
(
  customer customer_typ
);

INSERT INTO northwind.my_customers VALUES (customer_typ(1,'Edwig','Huisman',NULL,NULL,'NL','NL',1000,'edwig@hetnet.nl',NULL));

-- SELECT MUST specify at least a 3 part path to the attribute!!
SELECT c.customer.customer_id FROM my_customers c;

UPDATE northwind.my_customers c
   SET c.customer.phone_numbers = phone_list_typ('06-12345678','0172-98765')
 WHERE c.customer.customer_id = 1;

-- Flatting the tables by doing a carthesian product !!
-- 'column_value/object_value' is a predefined internal name from the 'varray'
SELECT c.customer.customer_id      AS id
      ,c.customer.cust_first_name  AS first_name
      ,p.column_value              AS phone_number
      ,ora_rowscn                  AS unique_row
  FROM my_customers c,
       TABLE(c.customer.phone_numbers) p
 WHERE c.customer.customer_id = 1;


-- DISCOVERY

SELECT sys_context('USERENV','DB_NAME') AS type_catalog
      ,t.owner    AS type_schema
      ,t.type_name
      ,'D'       AS type_usage
      ,1         AS ordinal
      ,''        AS attribute
      ,'VARRAY'  AS datatype
      ,0         AS notnull
      ,''        AS defaultvalue
      ,''        AS domaincheck
      ,''        AS remarks
      ,'<@>'     AS source
  FROM all_types t
 WHERE NOT EXISTS
     ( SELECT 1
         FROM all_type_attrs att
        WHERE t.owner     = att.owner
          AND t.type_name = att.type_name)
   AND t.persistable = 'YES'
   AND t.owner       = 'NORTHWIND'
   AND t.typecode    = 'COLLECTION'
UNION ALL
SELECT sys_context('USERENV','DB_NAME') AS type_catalog
      ,t.owner         AS type_schema
      ,t.type_name     AS type_name
      ,'C'             AS type_usage
      ,att.attr_no     AS ordinal
      ,att.attr_name   AS attribute
      ,att.attr_type_name ||
       CASE Nvl(att.Length,0)
            WHEN 0 THEN
                   CASE Nvl(att.precision,0)
                        WHEN 0 THEN ''
                        ELSE '(' || to_char(att.precision) || ',' || To_Char(att.scale) || ')'
                   END
            ELSE '(' || To_Char(att.Length) || ')'
       END  AS datatype
      ,0               AS notnull
      ,''              AS defaultvalue
      ,''              AS domaincheck
      ,''              AS remarks
      ,'<@>'           AS source
  FROM all_types t
       INNER JOIN all_type_attrs att ON (t.owner = att.owner
                                     AND t.type_name = att.type_name)
 WHERE t.persistable = 'YES'
   AND t.owner       = 'NORTHWIND'
 ORDER BY 1,2,3,5;

SELECT name
      ,line
      ,text
  FROM all_source
 WHERE TYPE  = 'TYPE'
   AND owner = 'NORTHWIND'
   AND name  = 'CUSTOMER_TYP'
 ORDER BY 1,2;
