//
// Copyright 2010 Centric IT Solutions. All rights reserved.
//
#include "Stdafx.h"
#include "Interval.h"
#include "SQLInfo.h"
#include <math.h>
#include <iosfwd>

#pragma warning (disable: 4244)

Interval::Interval(const Interval& interval)
         :m_ok(interval.m_ok)
{
  ZetWaarde(interval.m_intval);
}

Interval::Interval(const SQL_INTERVAL_STRUCT& intervalstruct)
         :m_ok(true)
{
  ZetWaarde(intervalstruct);
  return;
}

Interval::Interval(const CString& string
                  ,SQLINTERVAL type)
{
  m_ok = ZetInterval(string, type);
  return;
}

// Hulpfunctie: rond de gegeven Icd af op gehele getallen en geef het resultaat terug
// als long. "grens" => Alles dat binnen grens van de bovenliggende waarde afligt
// wordt naar boven afgerond, de rest naar beneden.
//
// [BSW] Wanneer bij de berekening een kommaloze waarde nodig is wordt
// WaardeVoorKomma gebruikt.
// Verder wordt de waarde altijd eerst vertaald naar de kleinste eenheid
// die voorkomt in het type, dan afgerond, en daarna pas worden de deelwaarden
// bepaald m.b.v. berekeningen in gehele getallen. Dit alles om afrondingsfouten
// te voorkomen die kunnen optreden als dergelijke berekeningen worden uitgevoerd
// met drijvende komma; bijvoorbeeld 1/12 is ongeveer 0.0833333333333333333... en
// dus kan met veel gemak 2/12 (+= 0.166666666666666666666666) keer twaalf gelijk
// worden aan 1.999999999999999999999999 i.p.v. 2. Door eerst maal 12 te doen en
// dan af te ronden wordt dit probleem omzeild.
static __int64 Rondaf(const double& icd, double grens = 0.5)
{
	static double een(1.0);
  if (icd - floor(icd) >= (1.0 - grens))
  {
    return (__int64)(floor(icd) + een);		
  }
  else
  {
    return (__int64)floor(icd);
  }
}

// Constructie van een interval vanuit de database
// Komt in Oracle als een ICD (gebrokengetal) binnen
//

Interval::Interval(const double& paramIcd, SQLINTERVAL type)
{
  double icd = paramIcd;
  memset(&m_intval,0,sizeof(SQL_INTERVAL_STRUCT));
  if (icd < 0.0)
  {
    icd = icd * -1.0;
    m_intval.interval_sign = -1;
  }
  else
  {
    m_intval.interval_sign = 1;
  }
  switch (type)
  {
    // Jaren, maanden zijn opgeslagen als maanden
    case SQL_IS_YEAR :
      m_intval.intval.year_month.year = (long)(floor(icd / 12.0));
      m_intval.interval_type = (SQLINTERVAL) type;
      break;
    // Maanden, opgeslagen als maanden
    case SQL_IS_MONTH:
      m_intval.intval.year_month.month = long(icd);
      m_intval.interval_type = (SQLINTERVAL) type;
      break;
    // Jaar-maand als hele maanden
    case SQL_IS_YEAR_TO_MONTH:
      m_intval.intval.year_month.year  = (long)(floor(icd / 12.0));
      m_intval.intval.year_month.month = (long)(icd - (12 * floor(icd / 12.0)));
      m_intval.interval_type = (SQLINTERVAL) type;
      break;
  }
  if(m_intval.interval_type)
  {
    m_waarde = (IntervalWaarde) paramIcd;
  }
  else
  {
    IntervalWaarde rest;
    // Zet het type alsnog
    m_intval.interval_type = (SQLINTERVAL) type;
    // Conversie (van DAGEN!) naar hele seconden
    rest = Rondaf(icd * (24.0 * 60 * 60)); 
    // Ook de waarde registreren
    m_waarde = rest * m_intval.interval_sign;
    switch(type)
    {
    // De rest is in dagen, ook als het niet een dagen-tot-xxx is!
    case SQL_IS_DAY:
        m_intval.intval.day_second.day = rest / (24 * 60 * 60);
      break;
    case SQL_IS_HOUR:
        m_intval.intval.day_second.hour   = rest / (60 * 60);
      break;
    case SQL_IS_MINUTE:
        m_intval.intval.day_second.minute = rest / 60;
      break;
    case SQL_IS_SECOND:
        m_intval.intval.day_second.second = rest;
      break;
    case SQL_IS_DAY_TO_HOUR:
        rest /= (60 * 60); // Uren
        m_intval.intval.day_second.day    = rest / 24;
        m_intval.intval.day_second.hour   = rest % 24;
      break;
    case SQL_IS_DAY_TO_MINUTE:
        rest /= 60; // Minuten
        m_intval.intval.day_second.minute = rest % 60;
        rest /= 60; // uren
        m_intval.intval.day_second.hour   = rest % 24;
        m_intval.intval.day_second.day    = rest / 24;
      break;
    case SQL_IS_DAY_TO_SECOND:
        m_intval.intval.day_second.second = rest % 60;
        rest /= 60; // Minuten
        m_intval.intval.day_second.minute = rest % 60;
        rest /= 60; // uren
        m_intval.intval.day_second.hour   = rest % 24;
        m_intval.intval.day_second.day    = rest / 24;
      break;
    case SQL_IS_HOUR_TO_MINUTE:
        rest /= 60; // Minuten
        m_intval.intval.day_second.minute = rest % 60;
        m_intval.intval.day_second.hour   = rest / 60;
      break;
    case SQL_IS_HOUR_TO_SECOND:
        m_intval.intval.day_second.second = rest % 60;
        rest /= 60; // Minuten
        m_intval.intval.day_second.minute = rest % 60;
        m_intval.intval.day_second.hour   = rest / 60;
      break;
    case SQL_IS_MINUTE_TO_SECOND:
        m_intval.intval.day_second.second = rest % 60;
        m_intval.intval.day_second.minute = rest / 60;
      break;
  }
  }
}


// BSW: Waarom verschilt deze constructor zo van de andere? Waar wordt hij voor gebruikt?
// EH:  Omdat hij zo van een andere geconstruct wordt, of omdat er expliciet 
//      een interval van een aantal seconden geconstruct wordt, bijvoorbeeld bij een
//      aftrekking van data, momenten of tijden
//      Het zijn dus nooit decimale dagen, zoals ze uit de database komen
Interval::Interval(const IntervalWaarde aantal
                  ,SQLINTERVAL type)
{
  // registreer de waarde
  m_waarde = aantal;
  
  IntervalWaarde werkaantal = aantal;
  IntervalWaarde rest;
  memset(&m_intval,0,sizeof(SQL_INTERVAL_STRUCT));

  if (aantal < 0 )
  {
    werkaantal *=  -1;
    m_intval.interval_sign = -1;
  }
  else
  {
    m_intval.interval_sign = 1;
  }
  switch (type)
  {
    case SQL_IS_YEAR :
      m_intval.intval.year_month.year = werkaantal / 12 ;
      break;
    case SQL_IS_MONTH:
      m_intval.intval.year_month.month = werkaantal;
      break;
    case SQL_IS_YEAR_TO_MONTH:
      m_intval.intval.year_month.year  = werkaantal / 12 ;
      m_intval.intval.year_month.month = werkaantal % 12;
      break;
    case SQL_IS_DAY:
      m_intval.intval.day_second.day = werkaantal / DAGEN_IN_SECONDEN;
      break;
    case SQL_IS_HOUR:
      m_intval.intval.day_second.hour = werkaantal / UREN_IN_SECONDEN;
      break;
    case SQL_IS_MINUTE:
      m_intval.intval.day_second.minute = werkaantal / 60;
      break;
    case SQL_IS_SECOND:
      m_intval.intval.day_second.second = werkaantal;
      break;
    case SQL_IS_DAY_TO_HOUR	:
      rest = werkaantal / (60 * 60); // In uren
      m_intval.intval.day_second.day  = rest / 24;
      m_intval.intval.day_second.hour = rest % 24;
      break;
    case SQL_IS_DAY_TO_MINUTE:
      rest = werkaantal / 60; // In minuten
      m_intval.intval.day_second.minute = rest % 60;
      rest /= 60; // In uren
      m_intval.intval.day_second.hour   = rest % 24;
      m_intval.intval.day_second.day    = rest / 24;
      break;
    case SQL_IS_DAY_TO_SECOND:
      m_intval.intval.day_second.second = werkaantal % 60;
      rest = werkaantal / 60; // In minuten
      m_intval.intval.day_second.minute = rest % 60;
      rest /= 60; // In uren
      m_intval.intval.day_second.hour   = rest % 24;
      m_intval.intval.day_second.day    = rest / 24;
      break;
    case SQL_IS_HOUR_TO_MINUTE:
      rest = werkaantal / 60; // In minuten
      m_intval.intval.day_second.minute = rest % 60;
      m_intval.intval.day_second.hour   = rest / 60;
      break;
    case SQL_IS_HOUR_TO_SECOND:
      m_intval.intval.day_second.second = werkaantal % 60;
      rest =  werkaantal / 60; // In minuten
      m_intval.intval.day_second.minute = rest % 60;
      m_intval.intval.day_second.hour   = rest / 60;
      break;
    case SQL_IS_MINUTE_TO_SECOND:
      m_intval.intval.day_second.second = werkaantal % 60;
      m_intval.intval.day_second.minute = werkaantal / 60;
      break;
  }
  m_intval.interval_type = (SQLINTERVAL) type;
}

// Rondaf workhourse
static long int round(const long double p_num)
{
  register long double num = p_num + 0.5;
  register long int getal = (long int) num;
  return getal;
}

Interval::Interval(const double p_aantal
                  ,SQLINTERVAL type)
{
  long double aantal = p_aantal;
  memset(&m_intval,0,sizeof(SQL_INTERVAL_STRUCT));

  if (aantal < 0 )
  {
    aantal *=  -1;
    m_intval.interval_sign = -1;
  }
  else
  {
    m_intval.interval_sign = 1;
  }
  m_intval.interval_type = (SQLINTERVAL) type; 
  switch (type)
  {
   case SQL_IS_YEAR :
      m_intval.intval.year_month.year = round(aantal);
      break;
    case SQL_IS_MONTH:
      m_intval.intval.year_month.month = round(aantal);
      break;
    case SQL_IS_YEAR_TO_MONTH:
      m_intval.intval.year_month.year  = (int)(aantal);
      aantal  = aantal - floor(aantal);
      aantal *= 12;
      m_intval.intval.year_month.month = round(aantal);
      break;
    case SQL_IS_DAY:
      m_intval.intval.day_second.day = round(aantal);
      break;
    case SQL_IS_HOUR:
      m_intval.intval.day_second.hour = round(aantal);
      break;
    case SQL_IS_MINUTE:
      m_intval.intval.day_second.minute = round(aantal);
      break;
    case SQL_IS_SECOND:
      m_intval.intval.day_second.second = round(aantal);
      break;
    case SQL_IS_DAY_TO_HOUR	:
      m_intval.intval.day_second.day  = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 24;
      m_intval.intval.day_second.hour = round(aantal);
      break;
    case SQL_IS_DAY_TO_MINUTE:
      m_intval.intval.day_second.day  = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 24.0;
      m_intval.intval.day_second.hour = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.minute = round(aantal);
      break;
    case SQL_IS_DAY_TO_SECOND:
      m_intval.intval.day_second.day  = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 24.0;
      m_intval.intval.day_second.hour = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.minute = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.second = round(aantal);
      break;
    case SQL_IS_HOUR_TO_MINUTE:
      m_intval.intval.day_second.hour   = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.minute = round(aantal);
      break;
    case SQL_IS_HOUR_TO_SECOND:
      m_intval.intval.day_second.hour = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.minute = (int)aantal;
      aantal = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.second = round(aantal);
      break;
    case SQL_IS_MINUTE_TO_SECOND:
      m_intval.intval.day_second.minute = (int) aantal;
      aantal  = aantal - floor(aantal);
      aantal *= 60;
      m_intval.intval.day_second.second = round(aantal);
      break;
  }
  Normaliseer();
  ZetWaarde(m_intval);
}

Interval::~Interval()
{
  return;
}

const bool
Interval::operator<(const Interval& interval) const
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return (AlsGetal() < interval.AlsGetal());
}

const bool
Interval::operator>(const Interval& interval) const
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return (AlsGetal() > interval.AlsGetal());
}

const bool
Interval::operator==(const Interval& interval) const
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return ( AlsGetal() == interval.AlsGetal() );
}

const bool
Interval::operator!=(const Interval& interval) const 
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return AlsGetal() != interval.AlsGetal();
}


const bool
Interval::operator<=(const Interval& interval) const
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return AlsGetal() <= interval.AlsGetal();
}


const bool
Interval::operator>=(const Interval& interval) const
{
  if(Resolutie() != interval.Resolutie())
  {
    throw CString("Vergelijking van twee niet-evenwaardige intervallen (jaar-maand / dag-seconde)");
  }
  return AlsGetal() >= interval.AlsGetal();
}


CString 
Interval::AlsString() const
{
  static char buffer[100];
  char *pnt = buffer;
  buffer[0] = 0;
  if (m_intval.interval_sign == -1)
  {
    strcpy_s(buffer,2,"-");
    ++pnt;
  }

  switch (m_intval.interval_type)
  {
    case SQL_IS_YEAR :          sprintf_s(pnt,100,"%d",m_intval.intval.year_month.year);
                                break;
    case SQL_IS_MONTH:          sprintf_s(pnt,100,"%d",m_intval.intval.year_month.month);
                                break;
    case SQL_IS_DAY:            sprintf_s(pnt,100,"%d",m_intval.intval.day_second.day);
                                break;
    case SQL_IS_HOUR:           sprintf_s(pnt,100,"%02d",m_intval.intval.day_second.hour);
                                break;
    case SQL_IS_MINUTE:         sprintf_s(pnt,100,"%02d",m_intval.intval.day_second.minute);
                                break;
    case SQL_IS_SECOND:         sprintf_s(pnt,100,"%02d",m_intval.intval.day_second.second);
                                break; 
    case SQL_IS_YEAR_TO_MONTH:  sprintf_s(pnt,100,"%d-%02d",m_intval.intval.year_month.year
                                                           ,m_intval.intval.year_month.month);
                                break;
    case SQL_IS_DAY_TO_HOUR	:   sprintf_s(pnt,100,"%d %02d",m_intval.intval.day_second.day
                                                           ,m_intval.intval.day_second.hour);
                                break;
    case SQL_IS_DAY_TO_MINUTE:  sprintf_s(pnt,100,"%d %02d:%02d",m_intval.intval.day_second.day
                                                                ,m_intval.intval.day_second.hour
                                                                ,m_intval.intval.day_second.minute);
                                break;
    case SQL_IS_DAY_TO_SECOND:  sprintf_s(pnt,100,"%d %02d:%02d:%02d",m_intval.intval.day_second.day
                                                                     ,m_intval.intval.day_second.hour
                                                                     ,m_intval.intval.day_second.minute
                                                                     ,m_intval.intval.day_second.second);
                                break;
    case SQL_IS_HOUR_TO_MINUTE: sprintf_s(pnt,100,"%02d:%02d",m_intval.intval.day_second.hour
                                                             ,m_intval.intval.day_second.minute);
                                break;
    case SQL_IS_HOUR_TO_SECOND: sprintf_s(pnt,100,"%02d:%02d:%02d",m_intval.intval.day_second.hour
                                                                  ,m_intval.intval.day_second.minute
                                                                  ,m_intval.intval.day_second.second);
                                  break;
    case SQL_IS_MINUTE_TO_SECOND: sprintf_s(pnt,100,"%02d:%02d",m_intval.intval.day_second.minute
                                                               ,m_intval.intval.day_second.second);
                                  break;
  }
  return buffer;
}

CString
LongNaarString(long p_waarde)
{
  CString value;
  value.Format("%d",p_waarde);
  return value;
}

//==============================================================================
// Deze functie geeft het interval als SQL-string.
//==============================================================================
CString Interval::AlsSQLString(bool p_waarde /*=true*/) const
{
  if(p_waarde)
  {
    CString value;
    value.Format("%g",AlsOpslagICD());
    return value;
  }
  CString quote("\'");
  switch (m_intval.interval_type)
  {
  case SQL_IS_YEAR :
    {
      return quote
           + LongNaarString(m_intval.intval.year_month.year) + " years"
           + quote;
    }
    break;
  case SQL_IS_MONTH:
    {
      return quote
           + LongNaarString(m_intval.intval.year_month.month) + " mons"
           + quote;
    }
    break;
  case SQL_IS_YEAR_TO_MONTH:
    {
      return quote
           + LongNaarString(m_intval.intval.year_month.year) + " years"
           + " " + LongNaarString(m_intval.intval.year_month.month) + " mons"
           + quote;
   }
    break;
  case SQL_IS_DAY:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.day) + " days"
           + quote;
    }
    break;
  case SQL_IS_HOUR:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.hour) + " hours"
           + quote;
    }
    break;
  case SQL_IS_MINUTE:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.minute) + " mins"
           + quote;
    }
    break;
  case SQL_IS_SECOND:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.second) + " secs"
           + quote;
    }
    break;
  case SQL_IS_DAY_TO_HOUR	:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.day) + " days "
           + LongNaarString(m_intval.intval.day_second.hour) + " hours"
           + quote;
    }
    break;
  case SQL_IS_DAY_TO_MINUTE:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.day) + " days "
           + LongNaarString(m_intval.intval.day_second.hour) + " hours "
           + LongNaarString(m_intval.intval.day_second.minute) + " mins"
           + quote;
    }
    break;
  case SQL_IS_DAY_TO_SECOND:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.day) + " days "
           + LongNaarString(m_intval.intval.day_second.hour) + " hours "
           + LongNaarString(m_intval.intval.day_second.minute) + " mins "
           + LongNaarString(m_intval.intval.day_second.second) + " secs"
           + quote;
    }
    break;
  case SQL_IS_HOUR_TO_MINUTE:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.hour) + " hours "
           + LongNaarString(m_intval.intval.day_second.minute) + " mins"
           + quote;
    }
    break;
  case SQL_IS_HOUR_TO_SECOND:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.hour) + " hours "
           + LongNaarString(m_intval.intval.day_second.minute) + " mins "
           + LongNaarString(m_intval.intval.day_second.second) + " secs"
           + quote;
    }
    break;
  case SQL_IS_MINUTE_TO_SECOND:
    {
      return quote
           + LongNaarString(m_intval.intval.day_second.minute) + " mins "
           + LongNaarString(m_intval.intval.day_second.second) + " secs"
           + quote;
    }
    break;
  default:
    return "";
  }
}

bool 
Interval::ZetInterval(const CString& string,SQLINTERVAL type)
{
#define INTERVAL_YEARS	0x040
#define	INTERVAL_MONTHS	0x020
#define INTERVAL_DAYS		0x010
#define INTERVAL_HOURS	0x004
#define INTERVAL_MINS		0x002
#define INTERVAL_SECS		0x001

  memset(&m_intval,0,sizeof(SQL_INTERVAL_STRUCT));
  m_waarde = 0;
  int internType = 0;

  CString separators    = ":./-";
  CString intervalletje = string;
  intervalletje.TrimLeft();
  intervalletje.TrimRight();
  // separators +=  sessie->GeefLocale().TijdChar();
  
  if (intervalletje == "0" || intervalletje == "")
  {
    m_intval.interval_type = (SQLINTERVAL) type;
    m_intval.interval_sign = 1;
    return m_ok = true;
  }
  int argumenten = 0;

  switch (type)
  {
    case SQL_IS_YEAR :              argumenten  = 1;
                                    internType |= INTERVAL_YEARS;
                                    break;
    case SQL_IS_MONTH:              argumenten  = 1;
                                    internType |= INTERVAL_MONTHS;
                                    break;
    case SQL_IS_DAY:                argumenten  = 1;
                                    internType |= INTERVAL_DAYS;
                                    break;
    case SQL_IS_HOUR:               argumenten  = 1;
                                    internType |= INTERVAL_HOURS;
                                    break;
    case SQL_IS_MINUTE:             argumenten  = 1;
                                    internType |= INTERVAL_MINS;
                                    break;
    case SQL_IS_SECOND:             argumenten  = 1;
                                    internType |= INTERVAL_SECS;
                                    break;
    case SQL_IS_YEAR_TO_MONTH:      argumenten  = 2;
                                    internType |= INTERVAL_YEARS;
                                    internType |= INTERVAL_MONTHS;
                                    break;
    case SQL_IS_DAY_TO_HOUR	:       argumenten  = 2;
                                    internType |= INTERVAL_DAYS;
                                    internType |= INTERVAL_HOURS;
                                    break;
    case SQL_IS_DAY_TO_MINUTE:      argumenten  = 3;
                                    internType |= INTERVAL_DAYS;
                                    internType |= INTERVAL_HOURS;
                                    internType |= INTERVAL_MINS;
                                    break;
    case SQL_IS_DAY_TO_SECOND:      argumenten  = 4;
                                    internType |= INTERVAL_DAYS;
                                    internType |= INTERVAL_HOURS;
                                    internType |= INTERVAL_MINS;
                                    internType |= INTERVAL_SECS;
                                    break;
    case SQL_IS_HOUR_TO_MINUTE:     argumenten  = 2;
                                    internType |= INTERVAL_HOURS;
                                    internType |= INTERVAL_MINS;
                                    break;
    case SQL_IS_HOUR_TO_SECOND:     argumenten  = 3;
                                    internType |= INTERVAL_HOURS;
                                    internType |= INTERVAL_MINS;
                                    internType |= INTERVAL_SECS;
                                    break;
    case SQL_IS_MINUTE_TO_SECOND:   argumenten  = 2;
                                    internType |= INTERVAL_MINS;
                                    internType |= INTERVAL_SECS;
                                    break;
  }
  // Formaat nu controleren
  if (!ControleerFormaat(intervalletje,argumenten,type))
  {
    ZetWaarde(m_intval);
    return m_ok = false;
  }
  const char* str = intervalletje.GetString();

  short	sign = 1;
	SQLUINTEGER	val1,val2,val3,val4;
	while(str && isspace(*str))
	{
		// Skip leading space to intervalletje
		str++;
	}
	if(!*str)
	{
    ZetWaarde(m_intval);
		return m_ok = true;
	}
	if(*str	== '-') 
	{
		sign = -1;
		str++;	
	}
	else if(*str == '+')
	{
		str++;	
	}
	val1 = val2 = val3 = val4 = 0;
	if(internType & (INTERVAL_YEARS | INTERVAL_MONTHS))
	{
		while(isdigit(*str))
		{
			val1 *= 10;
			val1 += *str++ - '0';
		}
		if(*str == '-') //  ||  *str == sessie->GeefLocale().DatumChar()) // Kan bijvoorbeeld '/' zijn
		{
			str++;
			while(isdigit(*str))
			{
				val2 *= 10;
				val2 += *str++ - '0';
			}
			m_intval.intval.year_month.year  = val1;
			m_intval.intval.year_month.month = val2;
		}
		else
		{
			if(internType & INTERVAL_YEARS)
			{
				m_intval.intval.year_month.year = val1;	
			}
			else // INTERVAL_MONTHS
			{
				m_intval.intval.year_month.month = val1;	
			}
		}
		if(*str)
    {
      ZetWaarde(m_intval);
			return m_ok = false;	
    }
	}
	else
	{
		if(internType & INTERVAL_DAYS)
		{
			while(isdigit(*str))
			{
				val1 *= 10;
				val1 += *str++ - '0';
			}
			m_intval.intval.day_second.day = val1;	
			if(internType & (INTERVAL_HOURS|INTERVAL_MINS|INTERVAL_SECS))
			{
				if(*str && *str++ != ' ')
        {
          ZetWaarde(m_intval);
          return m_ok = false;	
        }
			}
		}
		if(internType & INTERVAL_HOURS)
		{
			while(isdigit(*str))
			{
				val2 *= 10;
				val2 += *str++ - '0';
			}
			m_intval.intval.day_second.hour = val2;	
			if(internType & (INTERVAL_MINS|INTERVAL_SECS))
			{
				if(*str && separators.Find(*str) < 0)//  *str != ':')
        {
          ZetWaarde(m_intval);
          return m_ok = false;	
        }
        if(*str) ++str;
			}
		}
		if(internType & INTERVAL_MINS)
		{
			while(isdigit(*str))
			{
				val3 *= 10;
				val3 += *str++ - '0';
			}
			m_intval.intval.day_second.minute = val3;	
			if(internType & INTERVAL_SECS)
			{
				if(*str && separators.Find(*str) < 0) // *str++ != ':')
        {
          ZetWaarde(m_intval);
          return m_ok = false;	
        }
        if(*str) ++str;
			}
		}
		if(internType & INTERVAL_SECS)
		{
			while(isdigit(*str))
			{
				val4 *= 10;
				val4 += *str++ - '0';
			}
			m_intval.intval.day_second.second = val4;	
			if(*str)
			{
        ZetWaarde(m_intval);
				return m_ok = false;	
			}
		}
	}
  m_intval.interval_type = (SQLINTERVAL) type;
  m_intval.interval_sign = sign;

	Normaliseer();
  ZetWaarde(m_intval);
  return m_ok = true;
}

//	Deze functie normaliseert de interval, zodat bijvoorbeeld
//	een second:minute van 1:60 naar 2:00 gezet wordt.
void 
Interval::Normaliseer()
{
	int type = m_intval.interval_type;

  if(type == SQL_IS_YEAR_TO_MONTH)
  {
		
    while(m_intval.intval.year_month.month > 12)
		{
			SQLUINTEGER y, m;
			m = m_intval.intval.year_month.month;
			y = m / 12;
			m = m % 12;
			m_intval.intval.year_month.month  = m;
			m_intval.intval.year_month.year  += y;
		}
		return;
  }
  if(type == SQL_IS_HOUR_TO_SECOND   ||
     type == SQL_IS_MINUTE_TO_SECOND ||
     type == SQL_IS_DAY_TO_SECOND)
  {
		while(m_intval.intval.day_second.second >= 60)
		{
			SQLUINTEGER s, m;
			s = m_intval.intval.day_second.second;
			m = s / 60;
			s = s % 60;
			m_intval.intval.day_second.second  = s;
			m_intval.intval.day_second.minute += m;
		}
		while(type != SQL_IS_MINUTE_TO_SECOND && 
			    m_intval.intval.day_second.minute >= 60)
		{			
			SQLUINTEGER h, m;
			m = m_intval.intval.day_second.minute;
			h = m / 60;
			m = m % 60;
			m_intval.intval.day_second.minute = m;
			m_intval.intval.day_second.hour	 += h;
		}
		while(type == SQL_IS_DAY_TO_SECOND && 
		  	  m_intval.intval.day_second.hour >= 24)
		{
			SQLUINTEGER d, h;
			h = m_intval.intval.day_second.hour;
			d = h / 24;
			h = h % 24;
			m_intval.intval.day_second.hour = h;
			m_intval.intval.day_second.day += d;
		}
		return;
  }
  if(type == SQL_IS_HOUR_TO_MINUTE ||
     type == SQL_IS_DAY_TO_MINUTE)
  {	
		while(m_intval.intval.day_second.minute >= 60)
		{
			SQLUINTEGER m, h;
			m = m_intval.intval.day_second.minute;
			h = m / 60;	
			m = m % 60;
			m_intval.intval.day_second.minute  = m;
			m_intval.intval.day_second.hour   += h;
		}
		while(type == SQL_IS_DAY_TO_MINUTE &&
			    m_intval.intval.day_second.hour >= 24)
		{
			SQLUINTEGER h, d;
			h = m_intval.intval.day_second.hour;
			d = h / 24;	
			h = h % 24;
			m_intval.intval.day_second.hour = h;
			m_intval.intval.day_second.day += d;
		}
		return;
  }
  if(type == SQL_IS_DAY_TO_HOUR)
  {
    while (m_intval.intval.day_second.hour >= 24)
    {
			SQLUINTEGER h, d;
			h = m_intval.intval.day_second.hour;
			d = h / 24;
			h = h % 24;
			m_intval.intval.day_second.hour = h;
			m_intval.intval.day_second.day += d;
    }
		return;
  }
}

bool
Interval::ControleerFormaat(CString& intervalletje
                           ,const int argumenten
                           ,SQLINTERVAL type) const
{
  bool resultaat = true;
  int i = intervalletje.GetLength()-1;
  int tel = 0;
  int arg = 1;
  int streepjes = 0;
  int puntjes = 0;
  int spaces = 0;
  char c;
  bool voorloopmin = false;
  CString tintervalletje(intervalletje);

  CString separators    = ":./-";
//  separators +=  sessie->GeefLocale().TijdChar();

  // voorloop -/+ overslaan
  if (intervalletje.GetAt(0) == '-')
  {
    tintervalletje=intervalletje.Mid(1);
    voorloopmin = true;
    --i;
  }
  else if (intervalletje.GetAt(0) == '+')
  {
    tintervalletje = intervalletje.Mid(1);
    --i;
  }
  while (i>=0)
  {
    c = tintervalletje.GetAt(i);
    if (c != ' ' && (separators.Find(c) < 0))
    {
      tel++;
      if (tel > 10 && arg != argumenten && resultaat)
      {
        resultaat = false;
      }
    }
    else
    {
      tel = 0;
      arg++;
      if (arg > argumenten && resultaat)
      {
        resultaat = false;
      }
      if (c == '-') // || c == sessie->GeefLocale().DatumChar()) // Kan bijvoorbeeld '/' zijn
      {
        streepjes++;
      }
      else if (separators.Find(c) >= 0) //  c == ':')
      {
        puntjes++;
      }
      else
      {
        spaces++;
      }
    }
    i--;
  }
  if (!resultaat)
  {
    // Probeer het alsnog te herkennen en converteren.
    SQLINTERVAL huidigType = (SQLINTERVAL)0;

    if (streepjes == 1 && puntjes == 0 && spaces == 0 && arg == 2)
    {
      huidigType = SQL_IS_YEAR_TO_MONTH;
    }
    if (streepjes == 0 && puntjes == 0 && spaces == 1 && arg == 2)
    {
      huidigType = SQL_IS_DAY_TO_HOUR;
    }
    int spatie = intervalletje.Find(" ");
    int eerste = intervalletje.FindOneOf(separators); // " :"

    if (streepjes == 0 && puntjes == 1 && spaces == 1 && 
        arg == 3 && spatie == eerste)
    {
      huidigType = SQL_IS_DAY_TO_MINUTE;
    }
    if (streepjes == 0 && puntjes == 2 && spaces == 1 && 
        arg == 4 && spatie == eerste)
    {
      huidigType = SQL_IS_DAY_TO_SECOND;
    }
    if (streepjes == 0 && puntjes == 2 && spaces == 0)
    {
      huidigType = SQL_IS_HOUR_TO_SECOND;
    }
    if (huidigType == type)   // DJS Gewijzigd nav 128139
    {
      Interval* oud = new Interval(intervalletje,huidigType);
      Interval* conversie = new Interval(oud->AlsGetal(),type);
      intervalletje = conversie->AlsString();
      delete oud;
      delete conversie;
      resultaat = true;
    }
  }
  return resultaat;
}


// ostream& 
// operator<< (ostream& os, const Interval& interval)
// {
//   os << interval.AlsString();
//   return os;
// }

// Zo wordt'ie in de database opgeslagen
double
Interval::AlsOpslagICD() const
{
  switch (m_intval.interval_type)
  {
    case SQL_IS_YEAR :
    case SQL_IS_MONTH:
    case SQL_IS_YEAR_TO_MONTH:     return m_waarde;
    case SQL_IS_DAY:
    case SQL_IS_HOUR:
    case SQL_IS_MINUTE:
    case SQL_IS_SECOND:
    case SQL_IS_DAY_TO_HOUR	:
    case SQL_IS_DAY_TO_MINUTE:
    case SQL_IS_DAY_TO_SECOND:
    case SQL_IS_HOUR_TO_MINUTE:
    case SQL_IS_HOUR_TO_SECOND:
    case SQL_IS_MINUTE_TO_SECOND:  return ((double) m_waarde) / (long)DAGEN_IN_SECONDEN;
    default:                       break;
  }
  return 0.0;
}

// ICD als gebroken getal
double 
Interval::AlsICD() const
{
	double rekenWaarde = 0;
  switch (m_intval.interval_type)
  {
  case SQL_IS_YEAR :
    rekenWaarde = m_intval.intval.year_month.year;
    break;
  case SQL_IS_MONTH:
    rekenWaarde = m_intval.intval.year_month.month;
    break;
  case SQL_IS_DAY:
    rekenWaarde = m_intval.intval.day_second.day;
    break;
  case SQL_IS_HOUR:
    rekenWaarde = m_intval.intval.day_second.hour;
    break;
  case SQL_IS_MINUTE:
    rekenWaarde = m_intval.intval.day_second.minute;
    break;
  case SQL_IS_SECOND:
    rekenWaarde = m_intval.intval.day_second.second;
    break;
  case SQL_IS_YEAR_TO_MONTH:
    rekenWaarde = m_intval.intval.year_month.year;
    rekenWaarde += double(m_intval.intval.year_month.month) / 12;
    break;
  case SQL_IS_DAY_TO_HOUR	:
    rekenWaarde = m_intval.intval.day_second.day;
    rekenWaarde += double(m_intval.intval.day_second.hour) / 24;
    break;
  case SQL_IS_DAY_TO_MINUTE:
    rekenWaarde = m_intval.intval.day_second.day;
    rekenWaarde += double(m_intval.intval.day_second.hour) / 24;
    rekenWaarde += double(m_intval.intval.day_second.minute) / (24 * 60);
    break;
  case SQL_IS_DAY_TO_SECOND:
    rekenWaarde = m_intval.intval.day_second.day;
    rekenWaarde += double(m_intval.intval.day_second.hour) / 24;
    rekenWaarde += double(m_intval.intval.day_second.minute) / (24 * 60);
    rekenWaarde += double(m_intval.intval.day_second.second) / (24 * 60 * 60);
    break;
  case SQL_IS_HOUR_TO_MINUTE:
    rekenWaarde = m_intval.intval.day_second.hour;
    rekenWaarde += double(m_intval.intval.day_second.minute) / 60;
    break;
  case SQL_IS_HOUR_TO_SECOND:
    rekenWaarde = m_intval.intval.day_second.hour;
    rekenWaarde += double(m_intval.intval.day_second.minute) / 60;
    rekenWaarde += double(m_intval.intval.day_second.second) / (60 * 60);
    break;
  case SQL_IS_MINUTE_TO_SECOND:
    rekenWaarde = m_intval.intval.day_second.minute;
    rekenWaarde += double(m_intval.intval.day_second.second) / 60;
    break;
  default: throw CString("Onbekend soort interval");
           break;
  }
	rekenWaarde *= m_intval.interval_sign;
  return (double)rekenWaarde;
}

bool
Interval::ZetInterval(const SQL_INTERVAL_STRUCT& intervalstruct)
{
  ZetWaarde(intervalstruct);
  return true;
}

void
Interval::ZetWaarde(const SQL_INTERVAL_STRUCT& intervalstruct)
{
  m_intval = intervalstruct;
  m_waarde = 0;
  switch (m_intval.interval_type)
  {
  case SQL_IS_YEAR :    m_waarde = m_intval.intval.year_month.year * 12;
                        break;
  case SQL_IS_MONTH:    m_waarde = m_intval.intval.year_month.month;
                        break;
  case SQL_IS_DAY:      m_waarde = m_intval.intval.day_second.day * DAGEN_IN_SECONDEN;
                        break;
  case SQL_IS_HOUR:     m_waarde = m_intval.intval.day_second.hour * UREN_IN_SECONDEN;
                        break;
  case SQL_IS_MINUTE:   m_waarde = m_intval.intval.day_second.minute * 60;
                        break;
  case SQL_IS_SECOND:   m_waarde = m_intval.intval.day_second.second;
                        break;
  case SQL_IS_YEAR_TO_MONTH:    m_waarde  = m_intval.intval.year_month.year * 12;
                                m_waarde += m_intval.intval.year_month.month;
                                break;
  case SQL_IS_DAY_TO_HOUR	:     m_waarde  = m_intval.intval.day_second.day  * DAGEN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.hour * UREN_IN_SECONDEN;
                                break;
  case SQL_IS_DAY_TO_MINUTE:    m_waarde  = m_intval.intval.day_second.day    * DAGEN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.hour   * UREN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.minute * 60;
                                break;
  case SQL_IS_DAY_TO_SECOND:    m_waarde  = m_intval.intval.day_second.day    * DAGEN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.hour   * UREN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.minute * 60;
                                m_waarde += m_intval.intval.day_second.second;
                                break;
  case SQL_IS_HOUR_TO_MINUTE:   m_waarde  = m_intval.intval.day_second.hour   * UREN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.minute * 60;
                                break;
  case SQL_IS_HOUR_TO_SECOND:   m_waarde  = m_intval.intval.day_second.hour   * UREN_IN_SECONDEN;
                                m_waarde += m_intval.intval.day_second.minute * 60;
                                m_waarde += m_intval.intval.day_second.second;
                                break;
  case SQL_IS_MINUTE_TO_SECOND: m_waarde  = m_intval.intval.day_second.minute * 60;
                                m_waarde += m_intval.intval.day_second.second;
                                break;
  }
  m_waarde *= m_intval.interval_sign ;
}

Interval&
Interval::operator=(const Interval& interval)
{
  if (&interval != this)
    ZetWaarde(interval.m_intval);
  return *this;
}


const IntervalResolutie
Interval::Resolutie() const 
{
  switch (m_intval.interval_type)
  {
    case SQL_IS_YEAR :
    case SQL_IS_MONTH:
    case SQL_IS_YEAR_TO_MONTH:
      return maandResolutie;
    case SQL_IS_DAY:
    case SQL_IS_HOUR:
    case SQL_IS_MINUTE:
    case SQL_IS_SECOND:
    case SQL_IS_DAY_TO_HOUR	:
    case SQL_IS_DAY_TO_MINUTE:
    case SQL_IS_DAY_TO_SECOND:
    case SQL_IS_HOUR_TO_MINUTE:
    case SQL_IS_HOUR_TO_SECOND:
    case SQL_IS_MINUTE_TO_SECOND:
      return secondeResolutie;
    default:
      return onbekendeResolutie;
  }

}

const SQLINTERVAL
Interval::GeefType() const 
{
  return m_intval.interval_type;
}

const Interval
Interval::operator+(const Interval& interval) const
{

  if (Resolutie() != interval.Resolutie())
  {
    throw CString("Interval: optellen van niet-evenwaardige intervallen");
  }
  IntervalWaarde aantal = AlsGetal() + interval.AlsGetal();

  Interval arg1(aantal, m_intval.interval_type);
  return arg1;
}

const Interval
Interval::operator-(const Interval& interval) const
{
  if (Resolutie() != interval.Resolutie())
  {
    throw CString("Interval: Aftrekken van niet-evenwaardige intervallen");
  }
  IntervalWaarde aantal = AlsGetal() - interval.AlsGetal();

  Interval arg1(aantal, m_intval.interval_type);
  return arg1;
}

// const Tijd
// Interval::operator-(const Tijd& tijd) const
// {
//   if(Resolutie() == secondeResolutie)
//   {
//     IntervalWaarde aantal = AlsGetal() - tijd.AlsGetal();
//     Tijd arg1((long)aantal);
//     return arg1;
//   }
//   // Interval: Aftrekken van niet-evenwaardige intervallen
//   throw BasisExceptie(326);
// }

bool 
Interval::ControleerWaarde(const CString& inter, SQLINTERVAL type)
{
  ZetInterval(inter,type);
  return IsOk();
}

