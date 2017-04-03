DROP FUNCTION detail_check();

CREATE FUNCTION detail_check() returns trigger AS $$
BEGIN
  UPDATE detail
     SET amount = 100
   WHERE amount < 100;
END;
$$ LANGUAGE plpgsql



CREATE TRIGGER detail_update
AFTER UPDATE ON PUBLIC.detail
FOR EACH ROW
EXECUTE PROCEDURE detail_check();




SELECT event_object_catalog
      ,event_object_schema
      ,event_object_table
      ,trigger_name
      ,'' AS trigger_remarks
      ,0  AS trigger_position
      ,CASE action_timing
            WHEN 'AFTER' THEN FALSE ELSE TRUE
       END AS trigger_before
      ,CASE event_manipulation
            WHEN 'INSERT' THEN TRUE ELSE FALSE
      ,END AS trigger_insert
      ,CASE event_manipulation
            WHEN 'UPDATE' THEN TRUE ELSE FALSE
      ,END AS trigger_insert
      ,CASE event_manipulation
            WHEN 'DELETE' THEN TRUE ELSE FALSE
      ,END AS trigger_insert
      ,CASE event_manipulation
            WHEN 'SELECT' THEN TRUE ELSE FALSE
      ,END AS trigger_insert
      ,FALSE AS trigger_session
      ,FALSE AS trigger_transaction
      ,FALSE AS trigger_rollback
      ,''    AS trigger_referencing
      ,TRUE  AS trigger_enabled
      ,action_statment AS trigger_source
  FROM information_schema.triggers;



  bool      m_update      { false };    // UPDATE of a table
  bool      m_delete      { false };    // DELETE from a table
  bool      m_select      { false };    // SELECT from a table
  bool      m_session     { false };    // ON CONNECT / ON DISCONNECT
  bool      m_transaction { false };    // ON BEGIN   / ON COMMIT
  bool      m_rollback    { false };    // ON ROLLBACK
  CString   m_referencing;              // Referencing names OLD and NEW
  bool      m_enabled     { false };    // Enabled or disabled
  CString   m_source;                   // Source of the trigger
}
MetaTrigger;