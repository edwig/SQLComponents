////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/Common/Datum.h $
// $Author: ehuisman $
// 
// $log$
// Revision 162742 2011/04/21 16:48:50  ehuisman
//   RFC 418481 Gebruiker inactief maken in plaats van direct verwijderen.
// Revision 1 2011/03/08 13:54:39  ehuisman
//   Verdere centralisatie in BriefLibrary. Stap 2
// Revision 152685 2011/01/12 09:04:37  ehuisman
//   BRIEF4all.exe omnoemen naar BRIEFInterface.exe
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#pragma once
#include <time.h>
#include "SQLDatabase.h"
#include "Moment.h"

#define SECONDEN_PER_DAG (60 * 60 * 24)

// Datums worden opgeslagen als een MJD (Modified Julian Date = 17-11-1858)
// en werken correct voor 1-1-1 tot en met 31-12-9999 (ODBC Definitie)
typedef long DatumWaarde;

// In het geheugen is een datum een struct
struct _Datum
{
  short jaar;   // 0 - 9999 jaren
  char  maand;  // 1 - 12   maanden
  char  dag;    // 1 - 31   dagen
};

typedef struct _Datum DatumOpslag;

struct ParsedDate
{
  ParsedDate();

  double      m_dt;
  struct tm   m_tm;

  bool Parse(CString const& s);

private:

  bool TmFromOleDate();
};



class Datum
{
public:  
  Datum();

  // Datum gemaakt uit een dag,maand jaar
  Datum(const long dag, const long maand, const long jaar);

  // Datum gemaakt uit een andere datum
  Datum(const Datum& datum);

  // Datum gemaakt uit een string
  Datum(const CString& string);

  // Datum gemaakt uit een moment
  Datum(const Moment& moment);

  // Datum gemaakt uit een DATE_STRUCT
  Datum(const DATE_STRUCT* date);

  // Datum gemaakt uit een MJD datum waarde
  explicit Datum(DatumWaarde waarde);

  ~Datum();

  // Toekennings operator en methoden
  Datum& operator=(const Datum& datum);
  bool        ZetDatum(const CString& string); 
  bool        ZetDatum(long jaar, long maand, long dag);

  // Opvragen in diverse formaten
  CString     AlsString() const;
  DatumWaarde AlsGetal() const;
  CString     AlsSQLString(SQLDatabase* p_database);

  // Interne status van de datum
  bool        Valide()        const;
  bool        IsNull()        const;

  // Opvragen in deelformaten
  long        Jaar()          const;
  long        Maand()         const;
  long        Dag()           const;
  long        WeekDag()       const;
  CString     WeekDagNaam()   const;
  CString     MaandNaam()     const;
  int         WeekNummer()    const;
  long        DagenInMaand()  const;
 
  // Operaties op een datum
  Datum       PlusDagen     (const long aantalDagen)   const;
  Datum       PlusMaanden   (const long aantalMaanden) const;
  Datum       PlusJaren     (const long aantalJaren)   const;

  long        AantalJaren   (const Datum& datum) const; 
  long        AantalMaanden (const Datum& datum) const; 
  long        AantalDagen   (const Datum& datum) const;

  // Logische vergelijkings operatoren
  bool     operator <(const Datum& datum) const;
  bool     operator >(const Datum& datum) const;
  bool     operator==(const Datum& datum) const;
  bool     operator!=(const Datum& datum) const;
  bool     operator<=(const Datum& datum) const;
  bool     operator>=(const Datum& datum) const;

  // Opvragen van de huidige dag
  static Datum Vandaag();

  // Parsen van een string, ook voor de klasse Moment
  static bool ParseDatum(ParsedDate& pd, const CString& datum);

  // Ook voor gebruik vanuit de interface (static aanroepen)
  //
  // Bepaal een datum uit een interface string
  static Datum BepaalDatum(const CString& datum);

  // Haal een extra virtuele datum op (+/- <getal> <JAAR/MAAND/DAG/WEEK>)
  static bool HaalVirtueleDatumOp(CString        p_sign,
	 					                      CString        p_extraTime,
						                      long          p_interval,
					                        DatumOpslag&  p_temp);
  static void SplitStrDate(const  CString& p_strDate,
                                  CString& p_currentDate,
                                  CString& p_sign,
                                  CString& p_extraTime,
                                  int&     p_interval);
private:
  // Bereken m_mjd uit m_datum
  bool ZetMJD();

  // Reken een MJD terug naar de datum
  void MJDnaarDatum();

  // OPSLAG VAN DE DATUM 
  DatumWaarde m_mjd;    // Gemodificeerde Juliaanse dag
  DatumOpslag m_datum;  // Dag/maand/jaar
};      
 
/////////////////////////////////////////////////////////////////////////////
//
// Geef de rekenwaarde terug
//
inline DatumWaarde
Datum::AlsGetal() const
{
  return m_mjd;  
}
      
//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator=
//
// Omschrijving:   Assignment operator..
//
inline Datum&
Datum::operator=(const Datum& datum)
{
  if (&datum != this)
  {
    ZetDatum(datum.Jaar(),datum.Maand(),datum.Dag());
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Valide
//
// Omschrijving:   Test op validiteit van de datum instantie.
//
inline bool
Datum::Valide() const
{
  return (m_mjd != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::WeekDag
//
// Omschrijving:   Retourneert de weekdag van de instantie. 1=zondag, 2=maandag, ... , 
//                 7=zaterdag
//
inline long
Datum::WeekDag() const 
{ 
  return ((m_mjd +3)% 7);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Dag
//
// Omschrijving:   Retourneert de dag van de instantie. 
//
inline long
Datum::Dag() const 
{
  return m_datum.dag;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Maand
//
// Omschrijving:   Retourneert de maand van de instantie. 1=jan, 2=feb, ..., 12=dec. 
//
inline long
Datum::Maand() const 
{
  return m_datum.maand;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::Jaar
//
// Omschrijving:   Retourneert het jaar van de instantie in vier cijfers. 1997, 2001, etc.
//
inline long 
Datum::Jaar() const 
{
  return m_datum.jaar;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator<
//
inline bool
Datum::operator<(const Datum& datum) const
{
  return m_mjd < datum.m_mjd;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator>
//
inline bool
Datum::operator>(const Datum& datum) const
{
  return m_mjd > datum.m_mjd;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operato==
//
inline bool
Datum::operator==(const Datum& datum) const
{
  return m_mjd == datum.m_mjd;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator!=
//
// Omschrijving:   Vergelijkings operator. 
//
inline bool
Datum::operator!=(const Datum& datum) const 
{
  return !(operator==(datum));
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator<=
//
// Omschrijving:   Vergelijkings operator. 
//
inline bool
Datum::operator<=(const Datum& datum) const
{
  return !(operator>(datum));
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Datum::operator>=
//
// Omschrijving:   Vergelijkings operator. 
//
inline bool
Datum::operator>=(const Datum& datum) const
{
  return !(operator<(datum));
}
