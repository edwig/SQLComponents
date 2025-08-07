-- ORACLE PLAN_TABLE for SQL*Tools
--
CREATE TABLE plan_table
(
  statement_id    VARCHAR2(30)  NULL,
  timestamp       DATE          NULL,
  remarks         VARCHAR2(80)  NULL,
  operation       VARCHAR2(30)  NULL,
  options         VARCHAR2(30)  NULL,
  object_node     VARCHAR2(128) NULL,
  object_owner    VARCHAR2(30)  NULL,
  object_name     VARCHAR2(30)  NULL,
  object_instance NUMBER        NULL,
  object_type     VARCHAR2(30)  NULL,
  optimizer       VARCHAR2(255) NULL,
  search_columns  NUMBER        NULL,
  id              NUMBER        NULL,
  parent_id       NUMBER        NULL,
  position        NUMBER        NULL,
  cost            NUMBER        NULL,
  cardinality     NUMBER        NULL,
  bytes           NUMBER        NULL,
  other_tag       VARCHAR2(255) NULL,
  partition_start VARCHAR2(255) NULL,
  partition_stop  VARCHAR2(255) NULL,
  partition_id    NUMBER        NULL,
  other           LONG          NULL,
  distribution    VARCHAR2(30)  NULL
) ;


