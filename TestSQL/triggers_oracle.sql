CREATE OR REPLACE TRIGGER k2b.info_trigger
BEFORE INSERT OR UPDATE ON k2b.k2b_info_logboek
REFERENCING OLD AS oud NEW AS nieuw

BEGIN
  nieuw.log_error := 2 * nieuw.log_error;
END;

