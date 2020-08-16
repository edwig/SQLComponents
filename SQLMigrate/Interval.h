//
// Copyright 2010 Centric IT Solutions. All rights reserved.
//

#pragma once

#include <afxdb.h>
#include <sql.h>
#include <sqlext.h>

class DBInfo;

typedef enum IntervalResolutie
{
  onbekendeResolutie = 0
 ,maandResolutie     = 1
 ,secondeResolutie   = 2
};
          
#define DAGEN_IN_SECONDEN (24 * 60 * 60)
#define UREN_IN_SECONDEN  (60 * 60)

typedef __int64 IntervalWaarde;

class Interval
{      
public:  
  // Constructor rechstreeks vanuit een struct (ISO SQL databases)
  Interval(const SQL_INTERVAL_STRUCT& intervalstruct);

  // Constructor vanuit een ander interval
  Interval(const Interval& interval);

  // Constructor vanuit een interface string
  Interval(const CString& string, SQLINTERVAL type);
  
  // eenheid van aantal wisselt met het type
  Interval(const IntervalWaarde aantal, SQLINTERVAL type);

  // eenheid van icd is DAGEN. Dit is de versie die conversie vanaf OPSLAG doet.
  Interval(const double& icd, SQLINTERVAL type);

  // Constructie van een intern geheel/gebroken getal naar INTERVAL
  Interval(const double p_aantal,SQLINTERVAL type);

  // Algemene destructor
  ~Interval();

  // Retourneert de waarde als een intervalstruct
  SQL_INTERVAL_STRUCT GeefWaarde() const;

  // Retourneert het sub-type interval
  const SQLINTERVAL GeefType() const;

  bool ZetInterval(const CString& string, SQLINTERVAL type); 
  bool ZetInterval(const SQL_INTERVAL_STRUCT& intervalstruct);
  bool ControleerFormaat(CString& intervalletje, const int argumenten, SQLINTERVAL type) const;
  bool ControleerWaarde(CString const& intervalletje, SQLINTERVAL type);

  bool IsOk(void) const
  {
    return m_ok;
  }

  // Opvragen als string 
  CString AlsString() const;
  CString AlsSQLString(bool p_waarde = true) const;
  
  // Operatoren
        Interval& operator= (const Interval& interval);
  const Interval  operator+ (const Interval& interval) const;
  const Interval  operator- (const Interval& interval) const;
//const Tijd      operator- (const Tijd& tijd) const;
  const bool      operator< (const Interval& interval) const;
  const bool      operator> (const Interval& interval) const;
  const bool      operator==(const Interval& interval) const;
  const bool      operator!=(const Interval& interval) const;
  const bool      operator<=(const Interval& interval) const;
  const bool      operator>=(const Interval& interval) const;

  IntervalWaarde AlsGetal() const;
  
  // AlsICD geeft de representatie als ICD die logisch is voor de gebruiker
  // (in de eenheid van het grootste element in het intervaltype), maar AlsOpslagICD()
  // geeft de representatie zoals die in de database komt.
  double AlsICD() const;
  double AlsOpslagICD() const;

  // dagen-tot-seconden resolutie, of jaren-maandne resolutie
  const IntervalResolutie Resolutie() const;
	
	void Normaliseer();

private:
  void   ZetWaarde  (const SQL_INTERVAL_STRUCT& intervalstruct);

private:
  bool m_ok;                      // ok?
  IntervalWaarde      m_waarde;   // Rekenwaarde in hele seconden
  SQL_INTERVAL_STRUCT m_intval;   // jaren,maanden,dagen,uren,minuten,seconden
  // m_intval is in de layout van de ODBC driver (zodat de buffer gekopieerd kan worden)
};      

inline IntervalWaarde
Interval::AlsGetal() const
{
  return m_waarde;
}
  
inline SQL_INTERVAL_STRUCT
Interval::GeefWaarde() const
{
  return m_intval;
}


