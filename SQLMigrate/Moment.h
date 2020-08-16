////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/Common/Moment.h $
// $Author: ehuisman $
// 
// $log$
// Revision 1 2011/03/08 13:54:39  ehuisman
//   Verdere centralisatie in BriefLibrary. Stap 2
// Revision 152685 2011/01/12 09:04:38  ehuisman
//   BRIEF4all.exe omnoemen naar BRIEFInterface.exe
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#pragma once
#include <sqltypes.h>
#include <iostream>

class COleDateTime;
class Tijd;
class Datum;
class Moment;
class SQLDatabase;

#define SECONDEN_PER_DAG    (60 * 60 * 24)
#define SECONDEN_PER_UUR    (60*60)     // Een uur in seconden
#define SECONDEN_PER_MINUUT (60)        // Een minuut in seconden

// Het opslaan van een moment gebeurt door middel van het exacte aantal 
// seconden sinds MJD (Modified Julian Date). Hierdoor zijn momenten
// en datums intern met elkaar te verrekenen volgens
// DatumWaarde * SECONDEN_PER_DAG = MomentWaarde

struct MomentOpslag
{
  short jaar;      // 0 - 10000 jaar
  char maand;     // 1 - 12    maanden
  char dag;       // 1 - 31    dagen
  char uur;       // 0 - 23    uur
  char minuut;    // 0 - 59    minuten
  char seconde;   // 0 - 59    seconden
};

// Moment worden intern als hele seconden opgeslagen
typedef __int64 MomentWaarde;

#include <set>

class Moment
{
public:
  Moment(const Moment& Moment);
  Moment(int jaar, int maand,int dag,int uur,int min,int sec);
  Moment(const Datum& datum, const Tijd& tijd);
  Moment(const CString& momentstring,char* p_type = NULL);
  Moment(MomentWaarde waarde);
  Moment(TIMESTAMP_STRUCT stamp);
  
  Moment(){ ZetMoment(1900,1,1,0,0,0); }
  
public:
  bool Init(const CString& string);

  static Moment BepaalMoment   (const CString& str);
  static Moment BepaalMomentXml(const CString& str);


	static Moment HuidigMoment();
	static const Moment& ToekomstMoment();
  static const Moment& VerledenMoment();

public:
	bool operator==(const Moment& Moment) const;
	bool operator!=(const Moment& Moment) const;
	bool operator< (const Moment& Moment) const;
	bool operator> (const Moment& Moment) const;
	bool operator<=(const Moment& Moment) const;
	bool operator>=(const Moment& Moment) const;

  // Vergelijking zonder correct null-gedrag, dus twee null-momenten
  // zijn in deze vergelijkingen gelijk aan elkaar.
  bool   ExactGelijk    (const Moment& Moment) const;
  bool   NietExactGelijk(const Moment& Moment) const;

	int    Jaar()    const;
	int    Maand()   const;   // maand van het jaar   (1 = jan)
	int    Dag()     const;   // dag van de maand     (0-31)
	int    Uur()     const;   // uur van de dag       (0-23)
	int    Minuut()  const;   // minuut van het uur    (0-59)
	int    Seconde() const;   // seconde van de minuut (0-59)
	int    WeekDag() const;   // 1=zon, 2=maa, ..., 7=zat
  CString WeekDagNaam() const; // zondag, maandag, ..., zaterdag
  CString MaandNaam() const; // januari, februari, ..., december

  int     AantalJaren   (const Moment& Moment) const; 
  int     AantalMaanden (const Moment& Moment) const; 
  int     AantalDagen   (const Moment& Moment) const;
  int     AantalUren    (const Moment& Moment) const; 
  int     AantalMinuten (const Moment& Moment) const; 
  __int64 AantalSeconden(const Moment& Moment) const;

  Moment PlusDagen   (int aantalDagen)   const;
  Moment PlusMaanden (int aantalMaanden) const;
  Moment PlusJaren   (int aantalJaren)   const;
  Moment PlusUren    (int aantalUren)    const;
  Moment PlusMinuten (int aantalMin)     const;
  Moment PlusSeconden(__int64 aantalSec) const;

//   const Moment   operator-(const Interval& interval) const;
//   const Moment   operator+(const Interval& interval) const;
  //const Interval operator-(const Moment& moment) const;
  //const Interval operator-(const Datum& datum) const;

  const Moment& operator=(const Moment& Moment);

  friend std::ostream& operator<<(std::ostream& os, const Moment& mt);

	void   ZetMoment(int nYear,int nMonth,int nDay,
                   int nHour,int nMin,  int nSec);
  CString AlsString() const;
  CString AlsLeesString() const;
  CString AlsXMLString()  const;

  CString AlsSQLString(SQLDatabase* p_database) const;
private:
  Datum  AlsDatum() const;
  Tijd   AlsTijd() const;
public:
  void   ZetMoment(const CString& str);
  int    DagenInMaand() const;

  // Voor gebruik in Integer, ICD, Datum,Tijd en Interval
  MomentWaarde Waarde() const;
  //////////////////////////////////////////////////////
  // Static wordt gebruikt door Moment en Date
  static void SplitStrDate(const CString& strDate,
					                 CString&       CurrentDate,
					                 CString&       Sign,
					                 CString&       ExtraTime,
  						             int&         interval);


private:
  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // Private methoden
  //
  // Bereken de MomentWaarde uit een MomentOpslag
  void BerekenWaarde();
  // Bereken de MomentOpslag uit een MomentWaarde
  void Normaliseer();
  // Zet de momentwaarde op een expliciete waarde
  void ZetWaarde(MomentWaarde dubbel);
  // Bereken het verschil in jaren tussen twee momenten
  int  VerschilJaren(const Moment& beginmoment, const Moment& eindmoment) const;
  // Berekent het huidige aantal dagen in de maand
  int DagenInMaand(int jaar,int maand) const;
  // Bereken een moment uit een interface string en extra operanden
  static bool GetVirtualMoment(CString Sign
			                        ,CString ExtraTime
			                        ,int    interval
                              ,MomentOpslag& temp);

  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // datamembers
  //
  MomentOpslag m_hetmoment; // jaar/maand/dag/uur/minuut/seconde
	MomentWaarde m_waarde;		// Seconden sinds MJD
};

inline MomentWaarde
Moment::Waarde() const
{
  return m_waarde;
}

inline void
Moment::ZetWaarde(MomentWaarde waarde)
{
  m_waarde = waarde;
}

inline bool
Moment::ExactGelijk(const Moment& moment) const
{
  return m_waarde == moment.m_waarde;
}

inline bool
Moment::NietExactGelijk(const Moment& moment) const
{
  return m_waarde != moment.m_waarde;
}

inline int
Moment::DagenInMaand() const
{
  return DagenInMaand(Jaar(),Maand());
}


// Het komt in de historische verwerking vrij vaak
// voor dat momenten een "nullstatus" moeten kunnen
// hebben. Hiervoor dient deze klasse.
class MomentOfNull
{
  Moment m_moment;
  bool   m_gevuld;
public:
  MomentOfNull()
    : m_gevuld(false),
    m_moment(0) // willekeurige waarde
  {
  }

  MomentOfNull(const Moment& m)
    : m_gevuld(true),
    m_moment(m)
  {
  }

  bool operator==(const MomentOfNull& other) const
  {
    return (!m_gevuld && !other.m_gevuld) || (m_gevuld && other.m_gevuld && m_moment == other.m_moment);
  }
  bool operator!=(const MomentOfNull& other) const
  {
    return !operator==(other);
  }

  bool IsNull() const
  {
    return !m_gevuld;
  }

  Moment& AlsMoment()
  {
    if (!m_gevuld)
    {
      throw CString("Leeg moment opgevraagd");
    }
    return m_moment;
  }

  const Moment& AlsMoment() const
  {
    if (!m_gevuld)
    {
      throw CString("Leeg moment opgevraagd");
    }
    return m_moment;
  }

  void MaakNull()
  {
    m_gevuld = false;
  }

  void ZetMoment(const Moment& m)
  {
    m_moment = m;
    m_gevuld = true;    
  }
};


class MomentOfNu
{
public:
  bool isNu;
  Moment moment;

  MomentOfNu(bool is_nu, Moment het_moment = Moment()) : isNu(is_nu), moment(het_moment) {}
  MomentOfNu() : isNu(false), moment(0) {}

  Moment AlsMomentOfHuidigMoment() const { return isNu ? Moment::HuidigMoment() : moment; }

  bool operator==(const MomentOfNu& andere) const { return isNu == andere.isNu && (isNu || moment == andere.moment); }
  bool operator< (const MomentOfNu& andere) const { return AlsMomentOfHuidigMoment() < andere.AlsMomentOfHuidigMoment(); }
};

