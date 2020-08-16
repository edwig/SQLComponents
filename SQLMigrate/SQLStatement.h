////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLStatement.h $
// Auteurs:      B.P. Nebbeling
//
// Copyright (c) 1998 - 2007 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 22-11-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 2 $
//
#pragma once

class SQLStatement : public CObject
{
public :
  SQLStatement();
  SQLStatement(int Num, char *SQL, int Min, int Max);
 ~SQLStatement();
  virtual void    CheckRecords(int p_records);
          CString GetStatement();

private:
  int	    m_nummer;
  CString	m_omschrijving;
  CString	m_statement;
  int	    m_minAantal;
  int	    m_maxAantal;
  int	    m_opties;       // Never used in 6.2.0
  int	    m_soort;        // Never used in 6.2.0
  BOOL	  m_klantSpecifiek;
};

inline CString
SQLStatement::GetStatement()
{
  return m_statement;
}
