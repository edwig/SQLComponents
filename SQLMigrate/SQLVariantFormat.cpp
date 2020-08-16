////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLVariantFormat.cpp $
// Auteurs:      W.E. Huisman
//
// Copyright (c) 1995 - 2010 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 22-11-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 2 $
//
#include "stdafx.h"
#include "SQLVariantFormat.h"
#include <locale.h>

// string format number en valuta format functies
static bool ValutaInit = false;
static char DecimalSep[11];
static char DuizendSep[11];
static char strCurrency[11];
static int  DecimalSepLen;
static int  DuizendSepLen;
static int  strCurrencyLen;

static void 
InitValutaString()
{
  if(ValutaInit == false)
  {
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,  DecimalSep,  10);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, DuizendSep,  10);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, strCurrency, 10);
    DecimalSepLen  = strlen(DecimalSep);
    DuizendSepLen  = strlen(DuizendSep);
    strCurrencyLen = strlen(strCurrency);

    setlocale(LC_NUMERIC, "C");
    ValutaInit = true;
  }
}

SQLVariantFormat::SQLVariantFormat(SQLVariant* p_variant)
                 :m_variant(p_variant)
                 ,m_userStatus(0)
                 ,m_owner(false)
{
  InitValutaString();
}

SQLVariantFormat::~SQLVariantFormat()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
    m_variant = NULL;
  }
}

void
SQLVariantFormat::Reset()
{
  m_variant    = NULL;
  m_userStatus = 0;
  m_format     = "";
}

void
SQLVariantFormat::ReFormat()
{
  if(m_variant)
  {
    m_variant->GetAsString(m_format);
  }
}

// Alle woorden een eerste hoofdletter
void
SQLVariantFormat::StringInitCapital()
{
  bool doCapital = true;
  for(int ind = 0;ind < m_format.GetLength(); ++ind)
  {
    int ch = m_format.GetAt(ind);
    if(ch > 0 && ch <= 127)
    {
      if(isalpha(ch))
      {
        if(doCapital)
        {
          m_format.SetAt(ind,toupper(ch));
          doCapital = false;
        }
        else
        {
          m_format.SetAt(ind,tolower(ch));
        }
      }
      else
      {
        doCapital = true;
      }
    }
  }
}

// Eerste letter een hoofdletter van de hele string
void
SQLVariantFormat::StringStartCapital()
{
  bool doCapital = true;
  for(int ind = 0;ind < m_format.GetLength(); ++ind)
  {
    int ch = m_format.GetAt(ind);
    if(ch > 0 && ch <= 127)
    {
      if(isalpha(ch))
      {
        if(doCapital)
        {
          m_format.SetAt(ind,toupper(ch));
          doCapital = false;
        }
        else
        {
          m_format.SetAt(ind,tolower(ch));
        }
      }
    }
  }
}

bool
SQLVariantFormat::IsConstanteOfNummer(char p_separator)
{
  bool bDecimal  = false;
  bool bSpace    = false;
  bool bTeken    = false;
  bool bNumber   = false;
  CString string = m_format;

  string.Trim();
  int pos = 0;

  while(pos < string.GetLength())
  {
    int ch = string.GetAt(pos);
    if(isdigit(ch))
    {
      if(bSpace)
      {
        return false;
      }
      bNumber = true;
    }
    else if (ch == '-' || ch == '+')
    {
      if(bNumber || bSpace || bDecimal || bTeken)
      {
        return false;
      }
      bTeken = true;
    }
    else if (ch == p_separator)
    {
      if(bSpace || bDecimal)
      {
        return false;
      }
      bDecimal = true;
    }
    else if(isspace(ch))
    {
      bSpace = true;
    }
    else
    {
      return false;
    }
    // Volgend teken
    ++pos;
  }
  return true;
}

int 
SQLVariantFormat::StrValutaNLOmzetten(CString& p_string,bool p_enkelValuta)
{
  p_string.Replace(" ","");
  p_string.Replace("EUR.","");
  p_string.Replace("eur.","");
  p_string.Replace("E."  ,"");
  p_string.Replace("E"   ,"");

  if(! p_enkelValuta)
  {
    p_string.Replace(".","");
    p_string.Replace(",",".");
  }
  return p_string.GetLength();
}

double 
SQLVariantFormat::StringDoubleWaarde()
{
  InitValutaString();

  if(IsConstanteOfNummer())
  {
    return atof(m_format);
  }
  else
  {
    if(IsConstanteOfNummer(',') )
    {
      CString waarde = m_format;
      StrValutaNLOmzetten(waarde,false);
      return atof(waarde);
    }
    else
    {
      // Hopelijk werkt het !!
      return atof(m_format);
    }
  }
}

// Zet huidige datum en tijd
void
SQLVariantFormat::SetCurrentDate()
{
  CTime tm  = CTime::GetCurrentTime();
  m_format  = tm.Format("%d-%m-%Y %H:%M:%S");
  m_owner   = true;
  m_variant = new SQLVariant();
  m_variant->SetData(SQL_C_TIMESTAMP,m_format);
 }

// Format the date
// Lege string -> Korte windows notatie
// "@"         -> Lange windows notatie
// "d MMMM yyyy om |H:mm:ss|"  -> meest uitgebreide variant
int
SQLVariantFormat::FormatDate(CString p_pattern)
{
  char    buffer1[100] = "";
  char    buffer2[100] = "";
  CString dateFormat;
  CString timeFormat;
  DWORD   opties = 0;
  bool    doTime = false;

  // STAP 1: Controle datatypen en inhoud variant of string
  if(m_variant)
  {
    if(! m_variant->IsDateTimeType())
    {
      return ER_FormatDateTypeWaarde;
    }
  }
  else if(!m_format.IsEmpty())
  {
    // STAP 2: String naar DATE/TIME/TIMESTAMP
    m_variant = new SQLVariant();
    m_owner   = true;

    bool hasTime = p_pattern.Find('|') >= 0;
    bool hasDate = p_pattern.Left(1) != '|' || p_pattern.Right(1) != '|';
    if(hasDate && hasTime)
    {
      m_variant->SetData(SQL_C_TIMESTAMP,m_format);
    }
    else if(hasDate)
    {
      m_variant->SetData(SQL_C_DATE,m_format);
    }
    else if(hasTime)
    {
      m_variant->SetData(SQL_C_TIME,m_format);
    }
  }
  else
  {
    // Geen variant EN geen string
    return ER_FormatDateTypeStr;
  }
  // STAP 3: Opties bepalen
  doTime = p_pattern.Find('|') >= 0;

  if(p_pattern.IsEmpty())
  {
    opties = DATE_SHORTDATE;
  }
  else if(p_pattern == "@")
  {
    opties = DATE_LONGDATE;
    p_pattern = "";
  }
  else
  {
    // Prepareren voor API call
    p_pattern.Replace("jj","yy");
  }
  // STAP 4: Splitsen van patronen
  if(!p_pattern.IsEmpty())
  {
    int pos1 = p_pattern.Find('|');
    int pos2 = -1;
    if(pos1 >= 0)
    {
      pos2 = p_pattern.Find('|',pos1 + 1);
    }
    if(pos1 >= 0)
    {
      dateFormat = p_pattern.Left(pos1);
      timeFormat = p_pattern.Mid(pos1+1, pos2 - pos1 - 1);
    }
    else
    {
      // Alleen een datum
      dateFormat = p_pattern;
    }
  }
  // STAP 5: Verwerken datum deel
  int type = m_variant->GetDataType();
  if(type == SQL_C_DATE      || type == SQL_C_TIMESTAMP ||
     type == SQL_C_TYPE_DATE || type == SQL_C_TYPE_TIMESTAMP)
  {
    SYSTEMTIME str;
    memset((void*)&str,0,sizeof(SYSTEMTIME));
    DATE_STRUCT* date = m_variant->GetAsDate();
    str.wYear  = date->year;
    str.wMonth = date->month;
    str.wDay   = date->day;
    int buflen;
    if((buflen = GetDateFormat(0,opties,&str, ((opties != 0) ? NULL : (LPCTSTR)dateFormat) , buffer1, 100)) < 0)
    {
      buflen = 0;
    }
    buffer1[buflen] = 0;
  }
  // STAP 6: Verwerken tijd deel
  if(type == SQL_C_TIME      || type == SQL_C_TIMESTAMP ||
     type == SQL_C_TYPE_TIME || type == SQL_C_TYPE_TIMESTAMP)
  {
    SYSTEMTIME str;
    memset((void*)&str,0,sizeof(SYSTEMTIME));
    TIME_STRUCT* time = m_variant->GetAsTime();
    str.wHour   = time->hour;
    str.wMinute = time->minute;
    str.wSecond = time->second;
    int buflen;
    if((buflen = GetTimeFormat(0,0,&str,(timeFormat.GetLength() > 0 ? (LPCTSTR)timeFormat : NULL),buffer2,100)) < 0)
    {
      buflen = 0;
    }
    buffer2[buflen] = 0;
  }
  // STAP 7: Datum en tijd weer achter elkaar
  m_format = CString(buffer1) + CString(buffer2);
  return OK;
}

// Data optellen/aftrekken
int
SQLVariantFormat::DateCalculate(char p_bewerking,CString p_argument)
{
  int Getal;
  int Tel;
  int Incr;
  int Aantal;
  char GetalType;

  // STAP 1: Controle datatypen en inhoud variant of string
  if(m_variant)
  {
    if(! m_variant->IsDateTimeType())
    {
      return ER_FormatDateTypeWaarde;
    }
  }
  else if(!m_format.IsEmpty())
  {
    // STAP 2: String naar DATE/TIME/TIMESTAMP
    m_variant = new SQLVariant();
    m_owner   = true;

    m_variant->SetData(SQL_C_DATE,m_format);
  }
  else
  {
    // Geen variant EN geen string
    return ER_FormatDateTypeStr;
  }
  DATE_STRUCT*     date = m_variant->GetAsDate();
  COleDateTime     dDatum(date->year,date->month,date->day,0,0,0);
  COleDateTimeSpan dTimeSpan;

  Getal = 0;
  GetalType = 'D';
  if(p_bewerking != '+' && p_bewerking != '-' && p_bewerking != '~')
  {
    p_bewerking = '+';
  }
  if(p_bewerking == '+' || p_bewerking == '-')
  {

    Aantal = sscanf(p_argument, "%i %c", &Getal, &GetalType );

    //SetDateTimeSpan( long lDays, int nHours, int nMins, int nSecs );
    if (Getal != 0 )
    {
      if (Getal < 0 )
      {
        p_bewerking = ( p_bewerking == '+' ? '-' : '+' );
        Getal = abs(Getal);
      }
      GetalType = toupper(GetalType);

      switch (GetalType)
      {
      case 'D': dTimeSpan.SetDateTimeSpan(Getal, 0, 0, 0);
                if (p_bewerking == '+' )
                {
                  dDatum += dTimeSpan;
                }
                else
                {
                  dDatum -= dTimeSpan;
                }
                date->year  = dDatum.GetYear();
                date->month = dDatum.GetMonth();
                date->day   = dDatum.GetDay();
                m_variant->GetAsString(m_format);
                break;
      case 'M': Incr = ( p_bewerking == '+' ? 1 : -1 );
                for ( Tel = Getal ; Tel > 0 ; Tel--)
                {
                  date->month += Incr;
                  if(date->month > 12)
                  {
                    date->month = 1;
                    date->year += 1;
                  }
                  else if(date->month <= 0)
                  {
                    date->month = 12;
                    date->year -= 1;
                  }
                }
                m_variant->GetAsString(m_format);
                break;
      case 'Y':
      case 'J': if (p_bewerking == '+' )
                {
                  date->year += Getal;
                }
                else
                {
                  if(date->year < Getal)
                  {
                    date->year = 0;
                  }
                  else
                  {
                    date->year -= Getal;
                  }
                }
                m_variant->GetAsString(m_format);
                break;
      }
    }
  }
  if(p_bewerking == '~')
  {
    double dagen;
    COleDateTime DT;
    if(DT.ParseDateTime(p_argument))
    {
      dagen = DT - dDatum;
      m_format.Format("%i", (int)dagen);
    }
  }
  return OK;

}

// Formatteer volgens BRIEF4all
int
SQLVariantFormat::FormatNumber(CString p_format,bool p_currency)
{
  // standaard windows opmaak
  CString getal;

  if(m_variant)
  {
    if(m_variant->IsNumericType())
    {
      m_variant->GetAsString(m_format);
    }
    else
    {
      return ER_FormatNumberNoNumber;
    }
  }
  double waarde = StringDoubleWaarde();
  getal.Format("%f",waarde);

  if(p_format.IsEmpty())
  {
    char buffer[250];
    int	 bufLen = 0;
    if (p_currency)
    {
      bufLen = GetCurrencyFormat(LOCALE_USER_DEFAULT,0,getal,NULL,buffer,250);
    }
    else
    {
      bufLen = GetNumberFormat(LOCALE_USER_DEFAULT,0,getal,NULL,buffer,250);
    }
    if (bufLen <= 0)
    {
      bufLen = 0;
    }
    buffer[bufLen] = 0;
    m_format = buffer;
  }
  else
  {
    if(p_format.Find(';') < 0)
    {
      FormatNumberTemplate((char*)getal.GetString(),p_format);
    }
    else
    {
      // NIETS? Wat doen we hier?
    }
  }
  return OK;
}

CString 
SQLVariantFormat::FormatVariantForSQL(DatabaseType p_rdbmsType)
{
  CString text;
  int datatype    = m_variant->GetDataType();
  int toCdatatype = m_variant->FindDataTypeFromSQLType();

  if(toCdatatype == SQL_C_CHAR)
  {
    m_variant->GetAsString(text);
    text.Replace("\'","\'\'");
    text = "\'" + text + "\'";
  }
  else if(m_variant->IsNumericType())
  {
    m_variant->GetAsString(text);
  }
  else if(datatype == SQL_C_TIME || datatype == SQL_C_TYPE_TIME)
  {
    text = FormatVariantAsTime(p_rdbmsType);
  }
  else if(datatype == SQL_C_DATE || datatype == SQL_C_TYPE_DATE)
  {
    text = FormatVariantAsDate(p_rdbmsType);
  }
  else if(datatype == SQL_C_TIMESTAMP || datatype == SQL_C_TYPE_TIMESTAMP)
  {
    text = FormatVariantAsTimestamp(p_rdbmsType);
  }
  else if(m_variant->IsIntervalType())
  {
    text = FormatVariantAsInterval(p_rdbmsType);
  }
  else if(toCdatatype == SQL_C_GUID)
  {
    m_variant->GetAsString(text);
    text.TrimLeft("{");
    text.TrimRight("}");
    text = "{guid \'" + text + "\'}";
  }
  else
  {
    CString datatypeString = m_variant->FindDatatype(datatype);
    throw CString("Cannot prepare datatype for query: " + datatypeString);
  }
  return text;
}

CString
SQLVariantFormat::FormatVariantAsTime(DatabaseType p_type)
{
  CString time;
  switch(p_type)
  {
    case RDBMS_INFORMIX:      m_variant->GetAsString(time);
                              time = CString("DATETIME (") + time + ") HOUR TO SECOND";
                              break;
    case RDBMS_ORACLE:        m_variant->GetAsString(time);
                              time = CString("TO_DATE('") + time + ",'HH24:MI:SS')";
                              break;
//     case RDBMS_MSSQLSERVER:
//     case RDBMS_MSACCESS:
//     case RDBMS_MYSQL:
    case RDBMS_POSTGRESQL:
    case RDBMS_FIREBIRD:
//     case RDBMS_DB2:
//     case RDBMS_PROGRESS:
//     case RDBMS_PERVASIVE:
//     case RDBMS_SQLLITE:
//     case RDBMS_SYBASE:  
    case RDBMS_ODBC_STANDARD: m_variant->GetAsString(time);
                              time = CString("{t'") + time + "}";
                              break;
  }
  return time;
}

CString
SQLVariantFormat::FormatVariantAsDate(DatabaseType p_type)
{
  CString date;
  switch(p_type)
  {
    case RDBMS_INFORMIX:      m_variant->GetAsString(date);
                              date = CString("DATETIME (") + date + ") YEAR TO DAY";
                              break;
    case RDBMS_ORACLE:        m_variant->GetAsString(date);
                              date = CString("TO_DATE('") + date + ",'YYYY-MM-DD')";
                              break;
//     case RDBMS_MSSQLSERVER:
//     case RDBMS_MSACCESS:
//     case RDBMS_MYSQL:
    case RDBMS_POSTGRESQL:
    case RDBMS_FIREBIRD:
//     case RDBMS_DB2:
//     case RDBMS_PROGRESS:
//     case RDBMS_PERVASIVE:
//     case RDBMS_SQLLITE:
//     case RDBMS_SYBASE:  
    case RDBMS_ODBC_STANDARD: m_variant->GetAsString(date);
                              date = CString("{d'") + date + "}";
                              break;
  }
  return date;
}

CString
SQLVariantFormat::FormatVariantAsTimestamp(DatabaseType p_type)
{
  CString stamp;
  switch(p_type)
  {
    case RDBMS_INFORMIX:      m_variant->GetAsString(stamp);
                              stamp = CString("DATETIME (") + stamp + ") YEAR TO SECOND";
                              break;
    case RDBMS_ORACLE:        m_variant->GetAsString(stamp);
                              stamp = CString("TO_DATE('") + stamp + ",'YYYY-MM-DD HH24:MI:SS')";
                              break;
//     case RDBMS_MSSQLSERVER:
//     case RDBMS_MSACCESS:
//     case RDBMS_MYSQL:
    case RDBMS_POSTGRESQL:
    case RDBMS_FIREBIRD:
//     case RDBMS_DB2:
//     case RDBMS_PROGRESS:
//     case RDBMS_PERVASIVE:
//     case RDBMS_SQLLITE:
//     case RDBMS_SYBASE:  
    case RDBMS_ODBC_STANDARD: m_variant->GetAsString(stamp);
                              stamp = CString("{ts'") + stamp + "}";
                              break;
  }
  return stamp;
}

CString
SQLVariantFormat::FormatVariantAsInterval(DatabaseType p_type)
{
  CString ival;
  CString datatypeString = m_variant->FindDatatype(m_variant->GetDataType());
  datatypeString.TrimLeft("INTERVAL ");

  switch(p_type)
  {
    case RDBMS_INFORMIX:      m_variant->GetAsString(ival);
                              ival = "INTERVAL (" + ival + ") " + datatypeString;
                              break;
    case RDBMS_ORACLE:        // TODO
                              break;
//     case RDBMS_MSSQLSERVER:
//     case RDBMS_MSACCESS:
//     case RDBMS_MYSQL:
    case RDBMS_POSTGRESQL:
    case RDBMS_FIREBIRD:
//     case RDBMS_DB2:
//     case RDBMS_PROGRESS:
//     case RDBMS_PERVASIVE:
//     case RDBMS_SQLLITE:
//     case RDBMS_SYBASE:  
    case RDBMS_ODBC_STANDARD: ival = "{INTERVAL ";
                              CString interval;
                              m_variant->GetAsString(interval);
                              if(interval.GetAt(0) == '-')
                              {
                                interval = interval.Mid(1);
                                ival += "-\'";
                              }
                              else
                              {
                                ival += "+\'";
                              }
                              ival += interval + "\' ";
                              ival += datatypeString;
                              ival += "}";
                              break;
  }
  return ival;
}

// Intern formatteren van een getal volgens BRIEF4all
int
SQLVariantFormat::FormatNumberTemplate(char *Getal,const char *strNumFormat)
{
  char	strFormat[200];
  char	Buffer[250];
  int	  BufLen = 0;
  char	*pFormat;
  char	*pGetal;
  char	*pFormatStart;

  BOOL	bInFormat  = FALSE;
  BOOL	bFormat    = FALSE;
  BOOL	bInDecimal = FALSE;
  BOOL	bZero      = FALSE;
  BOOL	bNummer    = FALSE;
  BOOL	bNegatief  = FALSE;
  BOOL	bNul       = TRUE;
  BOOL	bAfronden  = FALSE;
  BOOL	bSign      = FALSE;
  char	InString   = '\0';
  char	LastChar   = '\0';
  char	LastOpmaak = '\0';
  int	  Tel;
  int	  Pos;
  char	FmtString[100];
  int	  FmtPos = 0;
  char	RestString[100];
  int	  RestPos = 0;

  BOOL	bGrouping     = FALSE;	// Wordt er grouping toegepast
  int	  nEersteGroup  = 0;		  // Grote van de eerste groep
  int	  nTweedeGroup  = 0;		  // grote van de tweede groep
  int	  nGroup        = 0;			// Huidige groep teller
  int	  nGroupSize    = 0;			// Grote van de repeating group

  int	  iGetalTrailing  = 0;
  int	  iGetalLeading   = 0;
  int	  iLeadingDigits  = 0;
  int	  iLeadingZero    = 0;
  int	  iTrailingDigits = 0;
  int	  iTrailingZero   = 0;
  int	  iAfronden       = 0;

  // opmaken en uitpluizen van het getal
  // Rommel verwijderen en de eerste controle
  bInDecimal = FALSE;
  bNummer    = FALSE;
  bSign      = FALSE;
  for (pFormatStart = Getal , pFormat = Getal ; *pFormat != '\0' ; *pFormat++)
  {
    if (!bNummer && (*pFormat == ' ' || *pFormat == '0' ))
    {
      continue;
    }
    if (strchr("1234567890", *pFormat) != NULL )
    {
      bNummer = TRUE;
      *pFormatStart++ = *pFormat;
    }
    else if (*pFormat == '.')
    {
      if (bInDecimal)
      {
        return ER_FormatNumberTemplateDecimal;
      }
      bInDecimal = TRUE;
      bNummer    = TRUE;
      *pFormatStart++ = '.';
    }
    else if (*pFormat == '-' || *pFormat == '+')
    {
      if ( !bSign )
      {
        bSign = TRUE;
        bNegatief = (*pFormat == '-');
        if (bNummer)
        {
          *pFormat = '\0';
          break;
        }
      }
      else
      {
        *pFormat = '\0';
        break;
      }
    }
    else
    {
      *pFormat = '\0';
      break;
    }
  }
  *pFormatStart = '\0';

  // uitpluizen van de format string
  // RestString[0] = '\0';
  // FmtString[0] = '\0';
  strncpy(strFormat, strNumFormat, 100);

  bInDecimal = FALSE;
  bNummer    = FALSE;
  for (pFormat = strFormat ; *pFormat != '\0' ; LastChar = *(pFormat++) )
  {
    if (LastChar == '\\')
    {
      RestPos--;
      RestString[RestPos++] = *pFormat;
      continue;
    }
    if (InString == '\0')
    {
      if (strchr("\"'", *pFormat) != NULL )
      {
        InString = *pFormat;
      }
      else if (strchr("@", *pFormat) != NULL )
      {
        // @# = Oorspronkelijke Nummer opmaak
        // @$ = Oorspronkelijke Valuta opmaak
        if (*(pFormat + 1) == '#')
        {
          if(GetNumberFormat(0,LOCALE_NOUSEROVERRIDE, Getal, NULL, Buffer, 100) > 0)
          {
            strcpy(&RestString[RestPos], Buffer);
            RestPos += strlen(Buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetNumberFormat;
          }
          pFormat++;
        }
        if (*(pFormat + 1) == '$')
        {
          if(GetCurrencyFormat(0, LOCALE_NOUSEROVERRIDE, Getal, NULL, Buffer, 100) > 0)
          {
            strcpy(&RestString[RestPos], Buffer);
            RestPos += strlen(Buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetCurrencyFormat;
          }
          pFormat++;
        }
      }
      else if (strchr("-+~^", *pFormat) != NULL )
      {
        switch (*pFormat)
        {
          case '^': strcpy(&RestString[RestPos], strCurrency);
                    RestPos += strlen(strCurrency);
                    break;
          case '+': RestString[RestPos++] = 1;
                    break;
          case '-': RestString[RestPos++] = 2;
                    break;
          case '~': RestString[RestPos++] = 3;
                    break;
        }
        bInFormat = FALSE;
      }
      else if (strchr("&#0xX.:,", *pFormat) != NULL )
      {
        if (! bInFormat)
        {
          if (bFormat)
          {
            return ER_FormatNumberTemplateFormat;
          }
          strcpy(&RestString[RestPos], "@*@");
          RestPos += 3;
          bFormat = TRUE;
          bInFormat = TRUE;
        }
        switch (*pFormat)
        {
          case '#':
          case '&': bNummer = TRUE;
                    if (bInDecimal)
                    {
                      iTrailingDigits++;
                    }
                    else
                    {
                      if (bZero)
                        return ER_FormatNumberTemplateZero;
                      iLeadingDigits++;
                      nGroup++;
                    }
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case '0': bZero = TRUE;
                    if (bInDecimal)
                    {
                      if (bNummer)
                        return ER_FormatNumberTemplateNummer ;
                      iTrailingZero++;
                      iTrailingDigits++;
                    }
                    else
                    {
                      iLeadingZero++;
                      iLeadingDigits++;
                      nGroup++;
                    }
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case 'x':
          case 'X': if (bInDecimal)
                    {
                      if (bAfronden)
                      {
                        return ER_FormatNumberTemplateAfronden;
                      }
                      bAfronden = TRUE;
                      if (*pFormat == 'x')
                      {
                        iTrailingDigits++;
                        iAfronden = iTrailingDigits;
                        FmtString[FmtPos++] = *pFormat;
                      }
                      else
                      {
                        iAfronden = iTrailingDigits;
                      }
                    }
                    else
                    {
                      return ER_FormatNumberTemplateNoDecimal;
                    }
                    break;
          case '.':
          case ':': bZero = FALSE;
                    bNummer = FALSE;
                    if (bInDecimal)
                      return ER_FormatNumberTemplateDecimal2;
                    bInDecimal = TRUE;
                    // groep vaststellen
                    if (bGrouping)
                    {
                      if (nEersteGroup == 0)
                      {
                        nEersteGroup = nGroup;
                      }
                      else if (nTweedeGroup == 0)
                      {
                        nTweedeGroup = nGroup;
                      }
                    }
                    nGroup = 0;
                    //
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case ',': if (bInDecimal)
                    {
                      return ER_FormatNumberTemplateDecimal3;
                    }
                    // groep vaststellen
                    bGrouping = TRUE;
                    if (nEersteGroup == 0)
                    {
                      nEersteGroup = nGroup;
                    }
                    else if (nTweedeGroup == 0)
                    {
                      nTweedeGroup = nGroup;
                    }
                    nGroup = 0;
                    //
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case '"':
          case '\'':InString = *pFormat;
                    break;
          default : RestString[RestPos++] = *pFormat;
                    break;
        }
      }
      else
      {
        RestString[RestPos++] = *pFormat;
      }
    }
    else
    { // instring
      if (*pFormat == InString)
      {
        InString = '\0';
      }
      else
      {
        RestString[RestPos++] = *pFormat;
      }
    }
  }
  // eventueel de laatste groep
  if (!bInDecimal && bGrouping)
  {
    if (nEersteGroup == 0)
    {
      nEersteGroup = nGroup;
    }
    else if (nTweedeGroup == 0)
    {
      nTweedeGroup = nGroup;
    }
  }
  if (bGrouping)
  {
    nGroupSize = max(nEersteGroup, nTweedeGroup);
  }
  // strings afsluiten
  RestString[RestPos] = '\0';
  FmtString[FmtPos] = '\0';

  /* @plaats opschonen getal */
  // Eventueel afronden indien noodzakelijk
  if (	bAfronden )
  {
    //strFormatLog += "[AFRONDEN DIE HAP]\r\n";
    NUMBERFMT NumFormat;
    NumFormat.NumDigits = iAfronden;
    NumFormat.LeadingZero = 0;
    NumFormat.Grouping = 0;
    NumFormat.lpDecimalSep = ".";
    NumFormat.lpThousandSep = ",";
    NumFormat.NegativeOrder = 0;
    BufLen = GetNumberFormat(0, 0, Getal, &NumFormat, Buffer, 100);
    if (BufLen <= 0)
    {
      return ER_FormatNumberTemplateBuflen;
    }
    strcpy(Getal, Buffer);
  }
  //
  // eigenschappen van het getal bepalen
  //
  bInDecimal = FALSE;
  for (pFormat = Getal ; *pFormat != '\0' ; *pFormat++)
  {
    if (strchr("0123456789", *pFormat) != NULL )
    {
      if (bInDecimal)
      {
        iGetalTrailing++;
      }
      else
      {
        iGetalLeading++;
      }
    }
    else if (*pFormat == '.')
    {
      bInDecimal = TRUE;
    }
    else
    {
      *pFormat = '\0';
      break;
    }
  }
  *pFormatStart = '\0';
  if (bInDecimal && iGetalTrailing > 0 )
  {
    while (iGetalTrailing > 0)
    {
      if (*(pFormatStart - 1) == '0' )
      {
        pFormatStart--;
        *pFormatStart = '\0';
        iGetalTrailing--;
      }
      else
      {
        break;
      }
    }
  }
  // Getal en formaat string op elkaar aanpassen
  // Getal grote als de format string
  if (iGetalLeading > iLeadingDigits)
  {
    // Aantal '#' toevoegen aan format string
    CString NewFormat;
    Pos = 0;
    nGroup = nEersteGroup;
    for (Tel = 0 ; Tel < (iGetalLeading - iLeadingDigits) ; Tel++ )
    {
      if (bGrouping)
      {
        if (nGroup == nGroupSize)
        {
          NewFormat = ',' + NewFormat;
          nGroup = 0;
        }
        NewFormat = '#' + NewFormat;
        nGroup++;
      }
      else
      {
        NewFormat += '#';
      }
    }
    NewFormat += FmtString;
    strcpy(FmtString, (LPCSTR)NewFormat);
  }
  else if (iGetalLeading < iLeadingDigits)
  {
    CString NewGetal;
    for (Tel = 0 ; Tel < (iLeadingDigits - iGetalLeading) ; Tel++ )
    {
      NewGetal += '0';
    }
    NewGetal += Getal;
    strcpy(Getal, (LPCSTR)NewGetal);
  }

  // Het Getal Formateren
  Pos = 0;
  RestPos = 0;
  bNummer = FALSE;
  LastOpmaak = '#';
  for (pFormat = FmtString, pGetal = Getal ; *pFormat != '\0' ; pFormat++)
  {
    switch (*pFormat)
    {
      case 'X' : break;
      case 'x' : *pFormat = LastOpmaak;
      case '#' :
      case '&' :
      case '0' :  LastOpmaak = *pFormat;
                  if ( (LastChar = *pGetal) == '\0' )
                    LastChar = '0';
                  else
                    pGetal++;
                  if (isdigit(LastChar))
                  {
                    if (*pFormat != '0')
                    {
                      bNul = FALSE;
                    }
                    if (!bNummer && LastChar == '0')
                    {
                      if (*pFormat == '#' )
                      {
                        Buffer[Pos++] = ' ';
                      }
                      else if (*pFormat == '&' )
                      {
                        Pos = Pos;
                      }
                      else
                      {
                        Buffer[Pos++] = '0';
                        bNummer = TRUE;
                      }
                    }
                    else
                    {
                      Buffer[Pos++] = LastChar;
                      bNummer = TRUE;
                    }
                  }
                  else
                  {
                    return ER_FormatNumberTemplateNoDigit;
                  }
                  break;
      case ',' :  if (bNummer)
                  {
                    strcpy(&Buffer[Pos], DuizendSep);
                    Pos += strlen(DuizendSep);
                  }
                  else
                  {
                    if (LastOpmaak != '&')
                      Buffer[Pos++] = ' ';
                  }
                  break;
      case '.' :
      case ':' :  if (*pGetal != '.' && *pGetal != '\0')
                  {
                    return ER_FormatNumberTemplateSomeKindOfError ;
                  }
                  *pGetal++;
                  if (iTrailingDigits > 0)
                  {
                    strcpy(&Buffer[Pos], DecimalSep);
                    Pos += strlen(DecimalSep);
                  }
                  if (*pFormat == ':')
                  {
                    Pos = 0;
                  }
                  break;
    }
  }
  Buffer[Pos] = '\0';
  CString string = RestString;
  string.Replace("@*@",Buffer);
  // Teken Plaatsen
  char tPlus[2]  = {1 , 0};
  char tMin[2]   = {2 , 0};
  char tTilde[2] = {3 , 0};
  if (bNul)
  {
    string.Replace(tPlus," ");
    string.Replace(tMin, " ");
    string.Replace(tTilde,"");
  }
  else if (bNegatief)
  {
    string.Replace(tPlus,"-");
    string.Replace(tMin, "-");
    string.Replace(tTilde,"-");
  }
  else
  {
    string.Replace(tPlus," ");
    string.Replace(tMin, "-");
    string.Replace(tTilde,"");
  }
  m_format = string;
  return OK;
}
