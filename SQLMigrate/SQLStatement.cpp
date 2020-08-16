////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLStatement.cpp $
// Auteurs:      B.P. Nebbeling
//
// Copyright (c) 1998 - 2007 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 22-11-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 2 $
//
#include "stdafx.h"
#include "SQLStatement.h"

SQLStatement::SQLStatement()
              :m_nummer(0)
              ,m_minAantal(0)
              ,m_maxAantal(0)
              ,m_opties(0)
              ,m_soort(0)
              ,m_klantSpecifiek(FALSE)
{
}

SQLStatement::SQLStatement(int Num, char *SQL, int Min, int Max)
              :m_nummer(Num)
              ,m_statement(SQL)
              ,m_minAantal(Min)
              ,m_maxAantal(Max)
              ,m_opties(0)
              ,m_soort(0)
              ,m_klantSpecifiek(FALSE)
{
}

// DTOR
SQLStatement::~SQLStatement()
{
}

void
SQLStatement::CheckRecords(int p_records)
{
	CString fout;
  if(m_minAantal > 0 && p_records < m_minAantal)
  {
		fout.Format("SQL %d heeft maar %d record(s) (te weinig) min = %d",m_nummer,p_records,m_minAantal);
		throw fout;
  }
  if(m_maxAantal > 0 && p_records > m_maxAantal)
  {
		fout.Format("SQL %d heeft %d records(s) (te veel) max = %i",m_nummer,p_records,m_maxAantal);
    throw fout;
  }
}

