////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/Common/Moment.cpp $
// $Author: ehuisman $
// 
// $log$
// Revision 162742 2011/03/12 19:47:43  ehuisman
//   Diverse kleine reparaties na ombouw. Archivering / geheugenbeheer / definfo / registry-keys
// Revision 1 2011/03/08 13:54:39  ehuisman
//   Verdere centralisatie in BriefLibrary. Stap 2
// Revision 152879 2011/03/08 08:38:16  ehuisman
//   Generieke SQLDatabase klasse naar common library verplaatst. BriefInterface / BriefServer hebben een eigen SQLDBase.
// Revision 152685 2011/01/12 09:04:38  ehuisman
//   BRIEF4all.exe omnoemen naar BRIEFInterface.exe
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#include "StdAfx.h"
#include <math.h>
#include "Moment.h"
#include "Tijd.h"
#include "Datum.h"
#include "SQLDatabase.h"
#include <strstream>
#include <ctime>

#pragma warning(disable: 4244)

/////////////////////////////////////////////////////////////////////////////
// Moment class HELPER definitions

// Aantal dagen aan het begin van de maand, 365 aan het eind van het jaar.
// Is gedefinieerd in Datum.cpp
extern int rijMaandDagen[];

// Construct uit een exact jaar,maand,dag,uur,min,sec
//
Moment::Moment(int jaar,int maand,int dag,
               int uur, int min,  int sec)
{
  ZetMoment(jaar, maand, dag, uur, min, sec);
}

// Construct uit een ander moment
//
Moment::Moment(const Moment& moment)
       :m_waarde(moment.m_waarde)
       ,m_hetmoment(moment.m_hetmoment)
{
}

// Construct uit een datum en een tijd
//
Moment::Moment(const Datum& datum, const Tijd& tijd)
{
  ZetMoment(datum.Jaar(), datum.Maand(), datum.Dag(),
            tijd.Uur(),   tijd.Minuut(), tijd.Seconde());
}

// Construct uit een expliciete waarde
// Wordt gebruikt om bijvoorbeeld vanuit een long of __int64 te constructen
//
Moment::Moment(MomentWaarde waarde)
       :m_waarde (waarde)
{
  // Vul jaar,maand,dag,uur,min,sec
  Normaliseer();
}

// Construct vanuit een string (b.v. vanuit de interface)
//
Moment::Moment(const CString& momentstring,char* p_type /*= NULL*/)
{
  if(p_type)
  {
    if(_strnicmp(p_type,"xml",3) == 0)
    {
      *this = BepaalMomentXml(momentstring);
      return;
    }
  }
  ZetMoment(momentstring);
}

// Construct vanuit een SQL TIMESTAMP_STRUCT
//
Moment::Moment(TIMESTAMP_STRUCT stamp)
{
  if(stamp.year == 0 && stamp.month == 0 && stamp.day == 0)
  {
    // Most likely a database NULL value
    ZetMoment(1,1,1,0,0,0);
  }
  else
  {
    ZetMoment(stamp.year,stamp.month, stamp.day,
              stamp.hour,stamp.minute,stamp.second);
  }
}

// Zet (wijzig) het moment
void
Moment::ZetMoment(int jaar,int maand,int dag,int uur,int min,int sec)
{
  m_hetmoment.jaar    = jaar;
  m_hetmoment.maand   = maand;
  m_hetmoment.dag     = dag;
  m_hetmoment.uur     = uur;
  m_hetmoment.minuut  = min;
  m_hetmoment.seconde = sec;

  BerekenWaarde();
}

void
Moment::BerekenWaarde()
{
  int jaar  = m_hetmoment.jaar;
  int maand = m_hetmoment.maand;
  int dag   = m_hetmoment.dag;

  // Validate jaar en maand en dag volgens de ODBC definitie
  
  if (jaar <= 0 || jaar >= 10000)
    throw CString("Jaartal moet liggen tussen 1 en 9999.");
  if (maand <= 0 || maand >= 13)
    throw CString("Maand moet liggen tussen 1 en 12.");
  if (m_hetmoment.uur < 0 || m_hetmoment.uur >= 24)
    throw CString("Uur moet liggen tussen 0 en 23.");
  if (m_hetmoment.minuut < 0 || m_hetmoment.minuut >= 60)
    throw CString("Minuut moet liggen tussen 0 en 59.");
  if (m_hetmoment.seconde < 0 || m_hetmoment.seconde >= 60)
    throw CString("Aantal seconden moet liggen tussen 0 en 59.");

  // Controle op Gregoriaanse definitie van maanden
  // Controleer op schrikkeljaar en bepaal dagen in de maand
  bool schrikkeljaar = ((jaar & 3) == 0) &&
                       ((jaar % 100) != 0 || (jaar % 400) == 0);

  long dagenInMaand = rijMaandDagen[maand] - rijMaandDagen[maand-1] +
                      ((schrikkeljaar && dag == 29 && maand == 2) ? 1 : 0);

  if (dag <= 0 || dag > dagenInMaand)
  {
    CString fout;
    fout.Format("Dag van de maand moet liggen tussen 1 en %d",dagenInMaand);
    throw fout;
  }
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
  // De correctie factor (Modified JD) = (1,720,994.5 - 2,400,000.5)
  m_waarde  = gregoriaansB + factorC + factorD + dag - 679006;
  m_waarde *= SECONDEN_PER_DAG;
  m_waarde += m_hetmoment.uur    * SECONDEN_PER_UUR +
              m_hetmoment.minuut * SECONDEN_PER_MINUUT +
              m_hetmoment.seconde;
}

void
Moment::Normaliseer()
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

  double JD = (m_waarde / SECONDEN_PER_DAG) + 2400001;
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
  m_hetmoment.dag   = factorC - factorE - (int)((double)factorG * 30.6001);
  m_hetmoment.maand = (factorG > 13) ? factorG - 13 : factorG - 1;
  m_hetmoment.jaar  = (m_hetmoment.maand > 2) ? factorD - 4716 : factorD - 4715;

  long seconden   = m_waarde % SECONDEN_PER_DAG;
  m_hetmoment.uur = seconden / SECONDEN_PER_UUR;
  int rest        = seconden % SECONDEN_PER_UUR;
  m_hetmoment.minuut  = rest / SECONDEN_PER_MINUUT;
  m_hetmoment.seconde = rest % SECONDEN_PER_MINUUT;
}

const Moment&
Moment::operator=(const Moment& moment)
{
  if (&moment != this)
  {
    m_waarde    = moment.m_waarde;
    m_hetmoment = moment.m_hetmoment;
  }
  return *this;
}

Moment
Moment::HuidigMoment()
{
  _tzset();
  time_t ltime;
  time(&ltime);
  struct tm now;
  localtime_s(&now,&ltime);
  Moment m(now.tm_year + 1900
          ,now.tm_mon  + 1
          ,now.tm_mday
          ,now.tm_hour 
          ,now.tm_min
          ,now.tm_sec);
  return m;
}

const Moment&
Moment::ToekomstMoment()
{
  // Grootste moment dat toegestaan is.
  static Moment m(9999,12,31,23,59,59);
  return m;
}

const Moment&
Moment::VerledenMoment()
{
  // Kleinste moment dat toegestaan is.
  static Moment m(1,1,1,0,0,0);
  return m;
}

bool
Moment::operator==(const Moment& moment) const
{
  return m_waarde == moment.m_waarde;
}

bool
Moment::operator!=(const Moment& moment) const
{
  return m_waarde != moment.m_waarde;
}

bool
Moment::operator<(const Moment& mom) const
{
  return m_waarde < mom.m_waarde;
}


bool
Moment::operator>(const Moment& mom) const
{
  return m_waarde > mom.m_waarde;
}

bool
Moment::operator<=(const Moment& mom) const
{
  return m_waarde <= mom.m_waarde;
}

bool
Moment::operator>=(const Moment& mom) const
{
  return m_waarde >= mom.m_waarde;
}

int
Moment::Jaar() const
{
  return m_hetmoment.jaar;
}

int
Moment::Maand() const
{
  return m_hetmoment.maand;
}

int
Moment::Dag() const
{
  return m_hetmoment.dag;
}

int
Moment::Uur() const
{
  return m_hetmoment.uur;
}

int
Moment::Minuut() const
{
  return m_hetmoment.minuut;
}

int
Moment::Seconde() const
{
  return m_hetmoment.seconde;
}

int
Moment::WeekDag() const
{
  MomentWaarde val = m_waarde / SECONDEN_PER_DAG;
  return (val + 3) % 7;
}

CString
Moment::WeekDagNaam() const
{
  switch (WeekDag())
  {
    case 0:  return "Zondag";
    case 1:  return "Maandag";
    case 2:  return "Dinsdag";
    case 3:  return "Woensdag";
    case 4:  return "Donderdag";
    case 5:  return "Vrijdag";
    case 6:  return "Zaterdag";
    default: return "";
  }
}

CString
Moment::MaandNaam() const
{
  long maandnr = m_hetmoment.maand;
  if (maandnr >= 1 && maandnr <= 12)
  {
    switch(maandnr)
    {
      case 1:  return "Januari";
      case 2:  return "Februari";
      case 3:  return "Maart";
      case 4:  return "April";
      case 5:  return "Mei";
      case 6:  return "Juni";
      case 7:  return "Juli";
      case 8:  return "Augustus";
      case 9:  return "September";
      case 10: return "Oktober";
      case 11: return "November";
      case 12: return "December";
    }
  }
  return "";
}

Moment
Moment::PlusDagen(int aantalDagen) const
{
  return Moment ((MomentWaarde)(m_waarde + aantalDagen * SECONDEN_PER_DAG));
}

Moment
Moment::PlusMaanden(int aantalMaanden) const
{
  if (aantalMaanden!=0)    // Als aantalMaanden = 0 return *this
  {
    int nieuweDag   = Dag();
    int nieuweMaand = Maand();
    int nieuwJaar   = Jaar();
    int uur = Uur();
    int min = Minuut();
    int sec = Seconde();

    nieuweMaand += aantalMaanden;
    if(nieuweMaand > 12)
    {
      nieuwJaar   +=  (nieuweMaand-1) / 12;
      nieuweMaand  = ((nieuweMaand-1) % 12) + 1;
    }
    else if(nieuweMaand < 1)
    {
      nieuwJaar  += (nieuweMaand - 12) / 12;
      nieuweMaand = 12 + (nieuweMaand % 12);
    }
    int dagenInNieuweMaand = DagenInMaand(nieuwJaar,nieuweMaand);
    if (nieuweDag > dagenInNieuweMaand)
    {
      nieuweDag = dagenInNieuweMaand;
    }
    return Moment(nieuwJaar, nieuweMaand, nieuweDag, uur, min, sec);
  }
  return *this;
}

int
Moment::DagenInMaand(int jaar,int maand) const
{
  bool schrikkeljaar = ((jaar & 3) == 0) && // als (jaar & 3) == 0 dan is jaar deelbaar
                       ((jaar % 100) != 0 || (jaar % 400) == 0);  // door 4 (binaire vergelijking).

  return rijMaandDagen[maand] - rijMaandDagen[maand-1] +
                      ((schrikkeljaar && maand == 2) ? 1 : 0);
}

Moment
Moment::PlusJaren(int aantalJaren) const
{
  try
  {
    return Moment(Jaar() + aantalJaren, Maand(), Dag(), Uur(), Minuut(), Seconde());
  }
  catch(CString s)
  {
    if (Maand() == 2 && Dag() == 29) // Correctie voor schrikkeljaar
    {
      // 29-2-1968 plus 1 jaar wordt 28-2-1969
      return Moment(Jaar() + aantalJaren, Maand(), Dag() - 1, Uur(), Minuut(), Seconde());
    }
    throw;  
  }
}

Moment
Moment::PlusUren(int aantalUren) const
{
  return Moment ((MomentWaarde)(m_waarde + aantalUren * SECONDEN_PER_UUR));
}


Moment
Moment::PlusMinuten(int aantalMinuten) const
{
  return Moment ((MomentWaarde)(m_waarde + aantalMinuten * SECONDEN_PER_MINUUT));
}


Moment
Moment::PlusSeconden(__int64 aantalSeconden) const
{
  return Moment ((MomentWaarde)(m_waarde + aantalSeconden));
}

int
Moment::AantalDagen(const Moment& moment) const
{
  return (moment.m_waarde - m_waarde) / SECONDEN_PER_DAG;
}

int
Moment::AantalMaanden(const Moment& moment) const
{
  if (*this == moment)
  {
    return 0;
  }
  Moment beginmoment = m_waarde < moment.m_waarde ? m_waarde : moment.m_waarde;
  Moment eindmoment  = m_waarde > moment.m_waarde ? m_waarde : moment.m_waarde;
  int beginDag   = beginmoment.Dag();
  int beginMaand = beginmoment.Maand();
  int beginJaar  = beginmoment.Jaar();
  int eindDag    = eindmoment.Dag();
  int eindMaand  = eindmoment.Maand();
  int eindJaar   = eindmoment.Jaar();

  int verschilJaren = eindJaar - beginJaar;
  int verschilMaanden = verschilJaren*12 + (eindMaand - beginMaand);
  if (beginDag > eindDag ||
      (beginDag == eindDag && (beginmoment.Uur() > eindmoment.Uur()||
       (beginmoment.Uur() == eindmoment.Uur() && (beginmoment.Minuut() > eindmoment.Minuut() ||
        (beginmoment.Minuut() == eindmoment.Minuut() && beginmoment.Seconde() > eindmoment.Seconde()))))))
  {
    --verschilMaanden;
  }
  if (*this > moment)
  {
    verschilMaanden = -verschilMaanden;
  }
  return verschilMaanden;
}

int
Moment::AantalJaren(const Moment& moment) const
{
  int verschilJaren = 0;
  if (!(*this == moment))
  {
    if (*this < moment)
    {
      verschilJaren = VerschilJaren(*this, moment);
    }
    else
    {
      verschilJaren = VerschilJaren(moment, *this);
      verschilJaren = -verschilJaren;
    }
  }
  return verschilJaren;
}


int
Moment::VerschilJaren(const Moment& beginmoment, const Moment& eindmoment) const
{
  int verschilJaren = eindmoment.Jaar()-beginmoment.Jaar();
  if (eindmoment.Maand()<beginmoment.Maand() ||
      (eindmoment.Maand()==beginmoment.Maand() && (beginmoment.Dag()>eindmoment.Dag () ||
       (beginmoment.Dag()==eindmoment.Dag() && (beginmoment.Uur()>eindmoment.Uur() ||
        (beginmoment.Uur()==eindmoment.Uur() && (beginmoment.Minuut()>eindmoment.Minuut() ||
         (beginmoment.Minuut()==eindmoment.Minuut() && beginmoment.Seconde()>eindmoment.Seconde()))))))))
  {
    --verschilJaren;
  }
  return verschilJaren;
}


int
Moment::AantalUren(const Moment& moment) const
{
  return AantalSeconden(moment) / SECONDEN_PER_UUR;
}


int
Moment::AantalMinuten(const Moment& moment) const
{
  return AantalSeconden(moment) / SECONDEN_PER_MINUUT;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// Moment::AantalSeconden
//
// Omschrijving:   Geeft het aantal seconden verschil tussen twee instanties. Het maximale
//                 tijdsverschil is 68 jaar (Dit is ongeveer gelijk aan de maximale waarde
//                 van een long in seconden). Als het verschil groter is dan 68 jaar dan is
//                 de wwarde ongedefinieerd.
//
__int64 
Moment::AantalSeconden(const Moment& moment) const
{
  return moment.m_waarde - m_waarde;
}

bool
Moment::Init(const CString& string)
{
  *this = BepaalMoment(string);
  return true;
}

/*static*/
Moment
Moment::BepaalMomentXml(const CString& str)
{
  int jaar     = 0;
  int maand    = 0;
  int dag      = 0;;
  int uur      = 0;
  int minuut   = 0;
  int seconde  = 0;
  int fraction = 0;
  CString temp(str);
  
  if(str.IsEmpty())
  {
    // Retourneer 1-jan-1900 als leeg moment
    return Moment(1900,1,1,0,0,0);
  }
  // Soms komt 'T' scheidingteken voor in XML (vanuit Suite4Publieksdiensten)
  temp.Replace("T"," ");
  int num = sscanf_s(temp,"%d-%d-%d %d:%d:%d.%d",&jaar,&maand,&dag,&uur,&minuut,&seconde,&fraction);
  // Negeer fraction
  if(num >= 6)
  {
    return Moment(jaar, maand, dag, uur, minuut, seconde);
  }
  // Onbekend formaat. voor nu: throw
  throw CString("Onbekend formaat voor moment: ") + str;
}

/*static*/
Moment
Moment::BepaalMoment(const CString& str)
{
  MomentOpslag temp;
  CString CurrentDate = "";
  CString Sign		 = "";
  CString ExtraTime   = "";
  int    interval = 0;

  Moment::SplitStrDate(str, CurrentDate, Sign, ExtraTime, interval);

  if(isalpha(CurrentDate.GetAt(0)))
  {
    // Snelheids optimalisatie: Alleen doen als eerste teken 
    // geen nummer is en dus een semantisch moment geparsed moet worden
    if (CurrentDate.CompareNoCase("HUIDIGETIJD")  == 0 ||  // NL
        CurrentDate.CompareNoCase("HUIDIGMOMENT") == 0 ||  // NL
        CurrentDate.CompareNoCase("NU")           == 0 ||  // NL
        CurrentDate.CompareNoCase("CURRRENT")     == 0 ||  // Eng
        CurrentDate.CompareNoCase("NOW")          == 0 )   // Eng
    {

      if (GetVirtualMoment(Sign, ExtraTime, interval, temp))
      {
        return Moment(temp.jaar,temp.maand,temp.dag,temp.uur,temp.minuut,temp.seconde);
      }
    }
    else if (CurrentDate.CompareNoCase("VANDAAG") == 0 ||
             CurrentDate.CompareNoCase("TODAY")   == 0 )
    { 
	    if (GetVirtualMoment(Sign, ExtraTime, interval, temp))
	    {
		    return Moment(temp.jaar, temp.maand, temp.dag, 0, 0, 0);
	    }
    }
    else if (CurrentDate.CompareNoCase("EVM") == 0 ||
             CurrentDate.CompareNoCase("FOM") == 0 )
    {
      // EersteVanMaand FirstOfMonth
      return Moment(HuidigMoment().Jaar()
                         ,HuidigMoment().Maand()
                         ,1
                         ,0,0,0);
    }
    else if (CurrentDate.CompareNoCase("LVM") == 0 ||
             CurrentDate.CompareNoCase("LOM") == 0 )
    {
      // EersteVanMaand / FirstOfMonth
      return Moment(HuidigMoment().Jaar()
                         ,HuidigMoment().Maand()
                         ,HuidigMoment().DagenInMaand()
                         ,23,59,59);
    }
    else if (CurrentDate.CompareNoCase("EVJ") == 0 ||
             CurrentDate.CompareNoCase("FOY") == 0 ||
             CurrentDate.CompareNoCase("HJ")  == 0 ||
             CurrentDate.CompareNoCase("CY")  == 0 )
    {
      // EersteVanJaar / FirstOfYear / HuidigJaar / CurrentYear
      return Moment(HuidigMoment().Jaar(),1,1,0,0,0);
    }
    else if (CurrentDate.CompareNoCase("LVJ") == 0 ||
             CurrentDate.CompareNoCase("LOY") == 0 )
    {
      // LaatsteVanJaar / LastOfYear
      return Moment(HuidigMoment().Jaar(),12,31,23,59,59);
    }
  }
  ParsedDate dat;
  if (!Datum::ParseDatum(dat,str))
  {
    throw CString("Verkeerd datum formaat: " + str);
  }

  long dag   = dat.m_tm.tm_mday;
  long maand = dat.m_tm.tm_mon;
  long jaar  = dat.m_tm.tm_year;

  long uur = dat.m_tm.tm_hour;
  long min = dat.m_tm.tm_min;
  long sec = dat.m_tm.tm_sec;

  //  Er was alleen een tijd
  if ((long)dat.m_dt == 0)   
  {
    Moment nu(HuidigMoment());
    jaar  = nu.Jaar();
    maand = nu.Maand();
    dag   = nu.Dag();
  }
  return Moment(jaar, maand, dag, uur, min, sec);
}

// Returnt bijvoorbeeld:
// Alle DB's: 13-06-1966 19:00:05
CString
Moment::AlsString() const
{
  // Controleer jaar volgens ODBC definitie
  // Anders gaat opslag in DB potentieel fout 
  int jaar = Jaar();
  if(jaar < 0)    jaar = 0;
  if(jaar > 9999) jaar = 9999;
  CString hetMoment;
  hetMoment.Format("%02d-%02d-%04d %02d:%02d:%02d"
                  ,Dag(),Maand(), jaar
                  ,Uur(),Minuut(),Seconde());
  return hetMoment;
}

CString
Moment::AlsXMLString() const
{
  int jaar = Jaar();
  if(jaar < 0)    jaar = 0;
  if(jaar > 9999) jaar = 9999;
  CString hetMoment;
  hetMoment.Format("%04d-%02d-%02dT%02d:%02d:%02d"
                  ,jaar, Maand(), Dag()
                  ,Uur(),Minuut(),Seconde());
  return hetMoment;
}

CString
Moment::AlsLeesString() const
{
  CString s;
  s.Format("%02d-%02d-%04d %02d:%02d:%02d",
          Dag(), Maand(), Jaar(), Uur(), Minuut(), Seconde());
  return s;
}

CString 
Moment::AlsSQLString(SQLDatabase* p_database) const
{
  DatabaseType type = p_database->GetDatabaseType();

  CString string;
  switch(type)
  {
    case RDBMS_ORACLE:    string.Format("TO_DATE('%02d-%02d-%04d %02d:%02d:%02d','DD-MM-YYYY HH24:MI:SS')"
                                       ,Dag(),Maand(),Jaar(),Uur(),Minuut(),Seconde());
                          break;
    case RDBMS_INFORMIX:  string.Format("'%s'",AlsString());
                          break;
    case RDBMS_ACCESS:    string.Format("(DateValue('%02d/%02d/%04d') + TimeValue('%2d:%2d:%2d'))"
                                       ,Maand(),Dag(),Jaar(),Uur(),Minuut(),Seconde());
                          break;
    case RDBMS_SQLSERVER: // Don't know yet
    case RDBMS_POSTGRESQL:// Don't know yet
    case RDBMS_FIREBIRD:  // Don't know yet
    default:              string = AlsString();
                          break;
  }
  return string;
}

Datum
Moment::AlsDatum() const
{
  return Datum(*this);
}

Tijd
Moment::AlsTijd() const
{
  return Tijd(*this);
}

void
Moment::ZetMoment(const CString& string)
{
  *this = BepaalMoment(string);
}

// STATIC wordt gebruikt door Moment en Datum
//
// Splitst moment-expressies in de vorm:
// VANDAAG + 2 DAG
// p_currentDate -> VANDAAG
// p_sign -> +
// p_extraTime -> DAG
// p_interval -> 2
// O.m. aangeroepen door DBRecord::CheckStandaardWaarde als een parameter 
// standaard waarde wortd gecontroleerd.
void Moment::SplitStrDate(const CString  &p_strDate,
					                CString& p_currentDate,
					                CString& p_sign,
					                CString& p_extraTime,
					                int&    p_interval)
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
			   p_interval  = atol(strIntrval);
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

bool 
Moment::GetVirtualMoment(CString Sign
                        ,CString ExtraTime
                        ,int    interval
                        ,MomentOpslag& temp)
{ 	
	Moment mom(HuidigMoment());
  if (!Sign.IsEmpty())
  {
    int factor = (Sign == "+") ? 1 : -1;
		if (Sign == "+" || Sign == "-")
		{
      ExtraTime.MakeUpper();
      if (ExtraTime == "" )      
      {
        return false;
      }
			else if (ExtraTime == "SEC"   || ExtraTime == "SECONDEN" )	  mom = mom.PlusSeconden(factor * interval);
			else if (ExtraTime == "MIN"   || ExtraTime == "MINUTEN" )		  mom = mom.PlusMinuten (factor * interval);
			else if (ExtraTime == "UUR"   || ExtraTime == "UREN" )		    mom = mom.PlusUren    (factor * interval);
			else if (ExtraTime == "DAG"   || ExtraTime == "DAGEN" )		    mom = mom.PlusDagen   (factor * interval);
			else if (ExtraTime == "WEEK"  || ExtraTime == "WEKEN" )			  mom = mom.PlusDagen   (factor * 7 * interval);
			else if (ExtraTime == "MAAND" || ExtraTime == "MAANDEN" )		  mom = mom.PlusMaanden (factor * interval);
			else if (ExtraTime == "JAAR"  || ExtraTime == "JAREN" ) 			mom = mom.PlusJaren   (factor * interval);
			else return false;
		}
    else
    {
      return false;
    }
  }
	temp = mom.m_hetmoment;
  return true;
}
