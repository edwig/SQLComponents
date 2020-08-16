////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLVariantFormat.h $
// Auteurs:      W.E. Huisman
//
// Copyright (c) 1995 - 2010 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 25-05-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 1 $
//
#pragma once
#include "SQLVariant.h"
#include "SQLDatabase.h"

// User status
#define USER_DISPLAY		1

class SQLVariantFormat
{
public:
  SQLVariantFormat(SQLVariant* p_variant);
 ~SQLVariantFormat();
  void        Reset();
  void        ReFormat();
  // Alle woorden een eerste hoofdletter
  void        StringInitCapital();
  // Eerste letter een hoofdletter van de hele string
  void        StringStartCapital();
  // Zet huidige datum en tijd
  void        SetCurrentDate();
  // Format the date
  int         FormatDate(CString p_pattern);
  // Is a constante of nummerieke
  bool        IsConstanteOfNummer(char p_separator = '.');
  // Testmatig '.' en ',' omzetten
  int         StrValutaNLOmzetten(CString& p_string,bool p_enkelValuta);
  // Waarde van de string als double
  double      StringDoubleWaarde();
  // Formatteer volgens BRIEF4all
  int         FormatNumber(CString p_format,bool p_currency);
  // Data optellen/aftrekken
  int         DateCalculate(char p_bewerking,CString p_argument);

  CString     FormatVariantForSQL(DatabaseType p_type);
  void        SetVariant(SQLVariant* p_variant);
  void        SetFormat(CString p_format);
  void        SetUserStatus(int p_status);
  SQLVariant* GetVariant();
  int         GetUserStatus();
  CString&    GetFormat();
private:
  CString     FormatVariantAsTime     (DatabaseType p_type);
  CString     FormatVariantAsDate     (DatabaseType p_type);
  CString     FormatVariantAsTimestamp(DatabaseType p_type);
  CString     FormatVariantAsInterval (DatabaseType p_type);
  // Intern formatteren van een getal volgens BRIEF4all
  int         FormatNumberTemplate(char *Getal,const char *strNumFormat);

  SQLVariant* m_variant;
  bool        m_owner;
  int         m_userStatus;
  CString     m_format;
};

inline void
SQLVariantFormat::SetVariant(SQLVariant* p_variant)
{
  m_variant = p_variant;
}

inline SQLVariant*
SQLVariantFormat::GetVariant()
{
  return m_variant;
}

inline void
SQLVariantFormat::SetUserStatus(int p_status)
{
  m_userStatus |= p_status;
}

inline int
SQLVariantFormat::GetUserStatus()
{
  return m_userStatus;
}

inline void
SQLVariantFormat::SetFormat(CString p_format)
{
  m_format = p_format;
}

inline CString&
SQLVariantFormat::GetFormat()
{
  return m_format;
}