-- TEST PACKAGE TO SEE WHAT COMES INTO THE CATALOG
--
RECREATE PACKAGE invoice
SQL SECURITY DEFINER
AS
BEGIN
    FUNCTION invoice_total(p_invoice INTEGER)
             returns DECIMAL(18,2);
    FUNCTION invoice_lines(p_invoice INTEGER)
             returns INTEGER;
    PROCEDURE invoice_text(p_invoice INTEGER)
              returns (p_line INTEGER,p_description VARCHAR(255));
END;



RECREATE PACKAGE BODY invoice
AS
BEGIN


FUNCTION invoice_total(p_invoice INTEGER)
RETURNS DECIMAL(18,2)
AS
  DECLARE total DECIMAL(18,2);
BEGIN
  SELECT Sum(amount)
    FROM detail
   WHERE mast_id = :p_invoice
    INTO total;
RETURN total;
END

FUNCTION invoice_lines(p_invoice INTEGER)
RETURNS INTEGER
AS
  DECLARE total INTEGER;
BEGIN
  SELECT Count(*)
    FROM detail
   WHERE mast_id = :p_invoice
    INTO total;
RETURN total;
END

PROCEDURE invoice_text(p_invoice INTEGER)
RETURNS(p_line INTEGER,p_description VARCHAR(255))
AS
BEGIN
FOR SELECT 0 AS line
          ,description
      FROM master
     WHERE id = :p_invoice
     UNION ALL
    SELECT line
          ,description
      FROM detail
     WHERE mast_id = :p_invoice
      INTO :p_line,:p_description
DO
  BEGIN
    SUSPEND;
  END
END


END;

COMMENT ON PACKAGE invoice IS 'Info from master/detail invoices';


SELECT invoice.invoice_lines(2) FROM rdb$database;
SELECT invoice.invoice_lines(1) FROM rdb$database;
SELECT * FROM invoice.invoice_text(1);


