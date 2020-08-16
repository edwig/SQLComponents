////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/Common/Datum.cpp $
// $Author: ehuisman $
// 
// $log$
// Revision 162742 2011/04/21 16:48:50  ehuisman
//   RFC 418481 Gebruiker inactief maken in plaats van direct verwijderen.
// Revision 1 2011/03/08 13:54:39  ehuisman
//   Verdere centralisatie in BriefLibrary. Stap 2
// Revision 152879 2011/03/08 08:38:16  ehuisman
//   Generieke SQLDatabase klasse naar common library verplaatst. BriefInterface / BriefServer hebben een eigen SQLDBase.
// Revision 152685 2011/01/12 09:04:37  ehuisman
//   BRIEF4all.exe omnoemen naar BRIEFInterface.exe
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#include "Stdafx.h"
#include "Datum.h"  
#include <windows.h>
#include <oleauto.h>
#include <cmath>
#include <ctime>

#pragma warning (disable: 4244)

// Aantal dagen aan het begin van de maand, 365 aan het eind van het jaar.
int rijMaandDagen[14] =
{
   0    // Januari
  ,31   // Februari 
  ,59   // Maart
  ,90   // April
  ,120  // Mei
  ,151  // Juni
  ,181  // Juli
  ,212  // Augustus
  ,243  // September
  ,273  // Oktober
  ,304  // November
  ,334  // December
  ,365  // 1 Jaar
  ,396  // Februari volgend jaar
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Datum
//
Datum::Datum(const Datum& datum)
{
  ZetDatum(datum.Jaar(),datum.Maand(),datum.Dag());
  return;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Datum
// Default datum die op all onderstende databases op is te slaan
// Grootste constraint hierbij is INFORMIX die niet verder terug gaat
//
Datum::Datum()
{
  ZetDatum(1899,12,30);
  return;
}

Datum::Datum(const long dag, const long maand, const long jaar)
{
  ZetDatum(jaar,maand,dag);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Datum
//
Datum::Datum(DatumWaarde waarde)
{
  m_mjd = waarde;
  MJDnaarDatum();  
}

// Datum gemaakt uit een string
Datum::Datum(const CString& string)
{
  if(string.IsEmpty())
  {
    ZetDatum(1899,12,30);
  }
  else
  {
    ZetDatum(string);
  }
}

// Datum gemaakt uit een moment
Datum::Datum(const Moment& moment)
{
  ZetDatum(moment.Jaar(),moment.Maand(),moment.Dag());
}

// Datum gemaakt uit een DATE_STRUCT
Datum::Datum(const DATE_STRUCT* date)
{
  ZetDatum((int)date->year,(int)date->month,(int)date->day);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::~Datum
//
// Omschrijving:   destructor. 
Datum::~Datum()
{
  return;
}

bool
Datum::IsNull() const
{
  // 15018 = MJD voor 31-12-1899 (null datum voor Informix)
  if(m_mjd == 0 || m_mjd == 15018)
  {
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::ZetDatum
//
// Omschrijving:   Zet de datum jaar-maand-dag in de datum instantie.
//
bool
Datum::ZetDatum(long jaar, long maand, long dag)
{
  m_datum.jaar  = jaar;
  m_datum.maand = maand;
  m_datum.dag   = dag;
  return ZetMJD();
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::ZetDatum
//
// Omschrijving:   Zet de waarde van de datum uit een string waarde.
//                 De string moet de format jjjj-mm-dd hebben.
//
bool 
Datum::ZetDatum(const CString& string)
{
  *this = BepaalDatum(string);
  return true;
}

// Bereken de rekenwaarde m_mjd uit m_datum
bool
Datum::ZetMJD()
{
  int jaar  = m_datum.jaar;
  int maand = m_datum.maand;
  int dag   = m_datum.dag;

  // Validate jaar en maand en dag volgens de ODBC definitie
  if (jaar >= 0 && jaar < 10000 &&
      maand > 0 && maand < 13   &&
      dag   > 0 && dag   < 32)
  {
    // Controle op Gregoriaanse definitie van maanden
    // Controleer op schrikkeljaar en bepaal dagen in de maand
    bool schrikkeljaar = ((jaar & 3) == 0) &&
                         ((jaar % 100) != 0 || (jaar % 400) == 0);

    long dagenInMaand = rijMaandDagen[maand] - rijMaandDagen[maand-1] +
                        ((schrikkeljaar && dag == 29 && maand == 2) ? 1 : 0);

    // Finish validating de datum
    if (dag <= dagenInMaand)
    {
      // Bereken Astronomisch Modified Juliaanse Dag Nummer (MJD)
      // Methode P.D-Smith: Practical Astronomy
      // Pagina 9: Paragraaf 4: Julian day numbers.
      // Zie ook
      // Robin.M. Green: Spherical Astronomy, pagina 250 e.v.
      if(maand < 3)
      {
        maand += 12;
        --jaar;
      }
      long gregoriaansA = 0;
      long gregoriaansB = 0;
      long factorC, factorD;
      if(jaar > 1582)
      {
        gregoriaansA = jaar / 100;
        gregoriaansB = 2 - gregoriaansA + (gregoriaansA / 4);
      }
      factorC = (365.25  * (double)jaar);
      factorD = (30.6001 * (double)(maand + 1));
      // De correctie factor (Modified JD) = (1720994.5 - 2400000.5)
      m_mjd = gregoriaansB + factorC + factorD + dag - 679006;
      return true;
    }
  }
  m_mjd = 0;
  return false;
}

// Reken de MJD (Modified Julian Day) terug naar een echte
// Gregoriaanse kalenderdatum
void
Datum::MJDnaarDatum()
{
  long gregoriaansA = 0;
  long gregoriaansB = 0;
  long factorC = 0;
  long factorD = 0;
  long factorE = 0;
  long factorG = 0;
  // Bereken Civil Day uit Modified Juliaanse Dag Nummer (MJD)
  // Methode P.D-Smith: Practical Astronomy
  // Pagina 11: Paragraaf 5: Converting Julian day number to the calendar date
  // Zie ook
  // Robin.M. Green: Spherical Astronomy, pagina 250 e.v.
  double JD = m_mjd + 2400001;
  if(JD > 2299160)
  {
    gregoriaansA = (JD - 1867216.25) / 36524.25;
    gregoriaansB = JD + 1 + gregoriaansA - (gregoriaansA / 4);
  }
  else
  {
    gregoriaansB = JD;
  }
  factorC = gregoriaansB + 1524;
  factorD = ((double)factorC - 122.1) / 365.25;
  factorE = (double)factorD * 365.25;
  factorG = ((double)(factorC - factorE)) / 30.6001;
  m_datum.dag   = factorC - factorE - (int)((double)factorG * 30.6001);
  m_datum.maand = (factorG > 13) ? factorG - 13 : factorG - 1;
  m_datum.jaar  = (m_datum.maand > 2) ? factorD - 4716 : factorD - 4715;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::DagenInMaand
//
// Omschrijving:   Geeft het aantal dagen van de maand. 
//
inline long
Datum::DagenInMaand() const
{
  long jaar  = m_datum.jaar;
  long maand = m_datum.maand;
  bool schrikkeljaar = ((jaar & 3) == 0) &&    // als (jaar & 3) == 0 dan is jaar deelbaar
    ((jaar % 100) != 0 || (jaar % 400) == 0);  // door 4 (binaire vergelijking).

  long dagenInMaand = rijMaandDagen[maand] - rijMaandDagen[maand-1] +
                      ((schrikkeljaar && maand == 2) ? 1 : 0);
  return dagenInMaand;
}

CString 
Datum::AlsString() const
{
  static char buffer[11];
  sprintf_s(buffer,11,"%02d-%02d-%04d", Dag(), Maand(), Jaar());
  return buffer;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Vandaag
//
// Omschrijving: De datum van vandaag.
//
inline Datum
Datum::Vandaag()
{
  _tzset();
  time_t ltime;
  time(&ltime);
  struct tm now;
  localtime_s(&now,&ltime);

  Datum vandaag;
  vandaag.m_datum.jaar  = now.tm_year + 1900;
  vandaag.m_datum.maand = now.tm_mon + 1;
  vandaag.m_datum.dag   = now.tm_mday;
  vandaag.ZetMJD();
  return vandaag;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::WeekDagNaam
//
// Omschrijving:   Retourneert de weekdagnaam van de instantie. 1=zondag, 2=maandag, ... , 
//                 7=zaterdag
//
CString
Datum::WeekDagNaam() const 
{ 
  if (m_mjd) // Valide?
  {
    // een op 0 (= zondag) gebaseerde dag
    int weekdag = (m_mjd + 3) % 7;
    switch (weekdag)
    {
      case 0: return "Zondag";
      case 1: return "Maandag";
      case 2: return "Dinsdag";
      case 3: return "Woensdag";
      case 4: return "Donderdag";
      case 5: return "Vrijdag";
      case 6: return "Zaterdag";
      default: return "";
    }
  }
  return "";
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::MaandNaam
//
// Omschrijving:   Retourneert de naam van de maand van de instantie. 1=januari, 2=februari, ... , 
//                 12=december
//
CString
Datum::MaandNaam() const 
{ 
  if (m_mjd)
  {
    if (m_datum.maand >= 1 && m_datum.maand <= 12)
    {
      switch(m_datum.maand)
      {
        case  1: return "januari";
        case  2: return "februari";
        case  3: return "maart";
        case  4: return "april";
        case  5: return "mei";
        case  6: return "juni";
        case  7: return "juli";
        case  8: return "augustus";
        case  9: return "september";
        case 10: return "oktober";
        case 11: return "november";
        case 12: return "december";
      }
    }
  }
  return "";
}

// Bepaal het weeknummer
int     
Datum::WeekNummer()  const
{
  if(m_mjd)
  {
    // Algoritme by: Tommy Skaue (CodeProject) / Peter-Paul Koch
    // ISO8601 Weeknumber (maandag is begin en 1)
    // Twee equivalente definities
    // 1) Week waarin 4 januari valt is week 1, of
    // 2) Week waarin eerste donderdag van het jaar valt is week 1 
    long a  = (14 - m_datum.maand) / 12;
    long y  = m_datum.jaar + 4800 - a;
    long m  = m_datum.maand + (12 * a) - 3;
    // Let op JD is misleidend. Eigenlijk Gregoriaanse dag
    long jd = m_datum.dag + (long)(((153*m)+2)/5) +
              (365 * y)   + (long)(y / 4) - (long)(y / 100) +
              (long)(y / 400) - 32045;

    long d4 = (jd + 31741 - (jd % 7)) % 146097 % 36524 % 1461;
    long l  = d4 / 1460;
    long d1 = ((d4-l) % 365) + l;
    long week = (d1 / 7) + 1;
    return week;
  }
  return 0;
}

Datum
Datum::BepaalDatum(const CString& datum)
{
  Datum  dt;
  DatumOpslag temp;
  CString CurrentDate = "";
  CString Sign		 = "";
  CString ExtraTime   = "";
  int    interval    = 0;
  bool   blnSuccess  = false;

  // Static split-string-date van moment aanroepen
  SplitStrDate(datum, CurrentDate, Sign, ExtraTime, interval);
  
  if(isalpha(CurrentDate.GetAt(0)))
  {
    // Snelheids optimalisatie: alleen als alphachars gevonden
    // Bij getallen direct door naar datum parser
    if (CurrentDate.CompareNoCase("VANDAAG")      == 0 ||
        CurrentDate.CompareNoCase("HUIDIGMOMENT") == 0 ||
        CurrentDate.CompareNoCase("NU")           == 0 ||
        CurrentDate.CompareNoCase("TODAY")        == 0 ||
        CurrentDate.CompareNoCase("CURRENT")      == 0 ||
        CurrentDate.CompareNoCase("NOW")          == 0 )
    {
      if (HaalVirtueleDatumOp(Sign, ExtraTime, interval, temp))
      {
        dt.ZetDatum(temp.jaar,temp.maand,temp.dag);
        blnSuccess = true;
      }
    }
    else if (CurrentDate.CompareNoCase("EVM") == 0 ||
             CurrentDate.CompareNoCase("FOM") == 0 )
    {
      // EersteVanMaand FirstOfMonth
      dt.ZetDatum(Vandaag().Jaar()
                 ,Vandaag().Maand()
                 ,1);
      blnSuccess = true;
    }
    else if (CurrentDate.CompareNoCase("LVM") == 0 ||
             CurrentDate.CompareNoCase("LOM") == 0 )
    {
      // EersteVanMaand / FirstOfMonth
      dt.ZetDatum(Vandaag().Jaar()
                 ,Vandaag().Maand()
                 ,Vandaag().DagenInMaand());
      blnSuccess = true;
    }
    else if (CurrentDate.CompareNoCase("EVJ") == 0 ||
             CurrentDate.CompareNoCase("FOY") == 0 ||
             CurrentDate.CompareNoCase("HJ")  == 0 ||
             CurrentDate.CompareNoCase("CY")  == 0 )
    {
      // EersteVanJaar / FirstOfYear / HuidigJaar / CurrentYear
      dt.ZetDatum(Vandaag().Jaar(),1,1);
      blnSuccess = true;
    }
    else if (CurrentDate.CompareNoCase("LVJ") == 0 ||
             CurrentDate.CompareNoCase("LOY") == 0 )
    {
      // LaatsteVanJaar / LastOfYear
      dt.ZetDatum(Vandaag().Jaar(),12,31);
      blnSuccess = true;
    }
  }
  if (!blnSuccess)
  {
    ParsedDate pd;
    if(!ParseDatum(pd, datum))
    {
      // Verkeerd datum formaat
      throw CString("Datum heeft een verkeerd formaat");
    }
    int dag   = pd.m_tm.tm_mday;
    int maand = pd.m_tm.tm_mon;
    int jaar  = pd.m_tm.tm_year;
    if (dag == 0 && maand == 0 && jaar == 0)
    {
      Datum huidig = Datum::Vandaag();
      jaar  = huidig.Jaar();
      maand = huidig.Maand();
      dag   = huidig.Dag();
    }
    dt.ZetDatum(jaar, maand, dag);
  }
  return dt;
}

// Haal een extra virtuele datum op (+/- <getal> <JAAR/MAAND/DAG/WEEK>)
bool 
Datum::HaalVirtueleDatumOp(CString       p_sign,
 							             CString       p_extraTime,
 							             long          p_interval,
 							             DatumOpslag&  p_temp)
{       
	Datum dt;
  Datum mom = Datum::Vandaag();

  if (!p_sign.IsEmpty())
  {
    p_extraTime.MakeUpper();
    if(p_sign != "-" && p_sign != "+")
    {
      return false;
    }
    int factor = (p_sign == "-") ? -1 : 1;
         if (p_extraTime == "DAG"   || p_extraTime == "DAGEN") 	 mom = mom.PlusDagen  (factor * p_interval);   
		else if (p_extraTime == "WEEK"  || p_extraTime == "WEKEN")   mom = mom.PlusDagen  (factor * p_interval * 7); 
		else if (p_extraTime == "MAAND" || p_extraTime == "MAANDEN") mom = mom.PlusMaanden(factor * p_interval);     
		else if (p_extraTime == "JAAR"  || p_extraTime == "JAREN" )  mom = mom.PlusJaren  (factor * p_interval);   
		else return false;
  }
  p_temp.jaar  = mom.Jaar();
  p_temp.maand = mom.Maand();
  p_temp.dag   = mom.Dag();

  return true;
}

bool
Datum::ParseDatum(ParsedDate& pd, const CString& datum)
{
  if(pd.Parse(datum))
  {
    return true;
  }

  LPCSTR p = (LPCSTR)datum;
  int len = 0;
  for(;*p != '\0';p++)
  {
    if (!isdigit(*p))
    {
      return false;
    }
    ++len;
  }

  SYSTEMTIME tm;
  ::GetLocalTime(&tm);
  
  CString newDate;
  if (len <= 2)
  {
    newDate.Format("%d %d %d", 
      atoi(datum.GetString()), 
      tm.wMonth, 
      tm.wYear);
  }
  else if (len <= 4)
  {
    char buf[10];
    newDate = datum.Left(2);
    newDate += " ";
    newDate += datum.Mid(2,2);
    newDate += " ";
    _itoa_s(tm.wYear,buf,10);
    newDate += buf;
  }
  else if (len <= 8)
  {
    newDate = datum.Left(2);
    newDate += " ";
    newDate += datum.Mid(2,2);
    newDate += " ";
    newDate += datum.Mid(4);
  }

  return pd.Parse(newDate);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::PlusDagen
//
// Omschrijving:   Telt een aantal dagen op bij een instantie en reourneert dit in een 
//                 nieuwe instantie. 
//
Datum
Datum::PlusDagen(const long aantalDagen) const
{
  Datum dat(*this);
  dat.m_mjd += aantalDagen;
  dat.MJDnaarDatum(); // Herberken de datum
  return dat;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::PlusMaanden
//
// Omschrijving:   Telt een aantal maanden op bij een instantie en reourneert dit in een 
//                 nieuwe instantie. 
//
Datum
Datum::PlusMaanden(const long aantalMaanden) const
{
  Datum dat(*this);
  if (aantalMaanden)
  {
    int maand = dat.Maand() + aantalMaanden - 1;
    if (maand >= 0)
    {
      dat.m_datum.maand  = (maand % 12) + 1;
      dat.m_datum.jaar  += maand / 12;
    }
    else
    {
      // Terug in de tijd, we kunnen geen % op een negatief getal doen want dat is undefined
      // (bijv. -5 % 12 = 7 of 5 afhankelijk van implementatie) dus we doen het anders.      
      maand = -maand - 1;                    // maand 0-based en positief maken.
      dat.m_datum.jaar  -= (maand / 12) + 1; // Zeker 1 jaar, aangezien -1 = december vorig jaar.
      dat.m_datum.maand = 12 - maand % 12; 
    }
    // Aantal dagen in de maand aftoppen
    int maxDag = dat.DagenInMaand();
    if(dat.m_datum.dag > maxDag)
    {
      dat.m_datum.dag = maxDag;
    }
    dat.ZetMJD();
  }
  return dat;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::PlusJaren
//
// Omschrijving:   Telt een aantal jaren op bij een instantie en reourneert dit in een 
//                 nieuwe instantie. 
//
Datum
Datum::PlusJaren(const long aantalJaren) const
{
  Datum dat(*this);
  dat.ZetDatum(Jaar() + aantalJaren,Maand(),Dag());
  if(!dat.Valide() && Maand() == 2 && Dag() == 29)
  {
    dat.ZetDatum(Jaar() + aantalJaren,Maand(),Dag() -1);
  }
  return dat;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::AantalDagen
//
// Omschrijving:   Geeft het aantal dagen verschil tussen twee instanties. 
//
long
Datum::AantalDagen(const Datum& datum) const
{
  return (datum.AlsGetal() - AlsGetal());
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::AantalMaanden
//
// Omschrijving:   Geeft het aantal maanden verschil tussen twee instanties. 
//
long
Datum::AantalMaanden(const Datum& datum) const
{
  return ((datum.Jaar() * 12) + datum.Maand()) - ((Jaar() * 12) + Maand());
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::AantalJaren
//
// Omschrijving:   Geeft het aantal jaren verschil tussen twee instanties. 
//
long 
Datum::AantalJaren(const Datum& datum) const
{
  return datum.Jaar() - Jaar();
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// Conversie van string naar datum
//

#define MIN_DATE                (-657434L)      // about year 100
#define MAX_DATE                2958466L        // year 10000 (exclusive!)
#define HALF_SECOND             (1.0/172800.0)  // Half a second, expressed in days

// One-based array of days in year at month start
static int _monthDays[13] =
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

ParsedDate::ParsedDate()
{
  m_dt = 0;
  memset(&m_tm, 0, sizeof(m_tm));
}

bool 
ParsedDate::TmFromOleDate()
{
	// The legal range does not actually span year 0 to 9999.
	if (m_dt >= MAX_DATE || m_dt < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = m_dt; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((m_dt > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	m_tm.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	m_tm.tm_yday = (int)n4Day + 1;
	m_tm.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			m_tm.tm_mon = 2;
			m_tm.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (m_tm.tm_mon = (n4Day >> 5) + 1;
		n4Day > _monthDays[m_tm.tm_mon]; m_tm.tm_mon++);

	m_tm.tm_mday = (int)(n4Day - _monthDays[m_tm.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		m_tm.tm_hour = m_tm.tm_min = m_tm.tm_sec = 0;
	else
	{
		m_tm.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		m_tm.tm_min = (int)nMinutesInDay % 60;
		m_tm.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

bool 
ParsedDate::Parse(CString const& s)
{
  BSTR bstrDate = s.AllocSysString();
  HRESULT hr = VarDateFromStr(bstrDate, LANG_USER_DEFAULT, 0, &m_dt);
  ::SysFreeString(bstrDate);
  if(FAILED(hr))
  {
    return false;
  }
  return TmFromOleDate();
}


// STATIC wordt gebruikt door Moment en Datum
//
// Splitst moment-expressies in de vorm:
// VANDAAG + 2 DAG
// p_currentDate -> VANDAAG
// p_sign -> +
// p_extraTime -> DAG
// p_interval -> 2
void 
Datum::SplitStrDate(const CString& p_strDate,
                          CString& p_currentDate,
                          CString& p_sign,
                          CString& p_extraTime,
                          int&     p_interval)
{
  bool blnFound     = false;
  p_currentDate     = "";
  p_sign			      = "";
  p_extraTime		    = "";
  p_interval		    = 0;
  CString strIntrval;

  int intLength = p_strDate.GetLength();

  for (int index = 0; index < intLength; index++)
  {
    CString temp(p_strDate.GetAt(index),1);
    if (temp.Compare(" ") != 0)   // geen spatie
    {
      if ((p_strDate.GetAt(index) >= '0') && (p_strDate.GetAt(index) <= '9'))
      {
        strIntrval += p_strDate.GetAt(index);
        p_interval = atoi(strIntrval); // strIntrval.StringNaarLong(strIntrval);
      }
      else if (p_strDate.GetAt(index) == '+' || p_strDate.GetAt(index) == '-')
      {
        p_sign += p_strDate.GetAt(index);
        blnFound = true;
      }
      else if (blnFound)
      {
        p_extraTime += p_strDate.GetAt(index);
      }
      else
      {
        p_currentDate += p_strDate.GetAt(index);
      }
    }
  }
  if (p_currentDate.IsEmpty())
  {
    p_currentDate = p_strDate;
  }
}

CString     
Datum::AlsSQLString(SQLDatabase* p_database)
{
  DatabaseType type = p_database->GetDatabaseType();

  CString string;
  switch(type)
  {
    case RDBMS_ORACLE:    string.Format("TO_DATE('%2.2d-%2.2d-%4.4d','DD-MM-YYYY')",Dag(),Maand(),Jaar());
                          break;
    case RDBMS_INFORMIX:  string.Format("'%s'",AlsString());
                          break;
    case RDBMS_ACCESS:    string.Format("DateValue('%2.2d/%2.2d/%4.4d')",Maand(),Dag(),Jaar());
                          break;
    case RDBMS_SQLSERVER: // Don't know yet
    case RDBMS_POSTGRESQL:// Don't know yet
    case RDBMS_FIREBIRD:  // Don't know yet
    default:              string = AlsString();
                          break;
  }
  return string;
}
