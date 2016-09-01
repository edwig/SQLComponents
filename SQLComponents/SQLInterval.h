////////////////////////////////////////////////////////////////////////
//
// File: SQLInterval.h
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#pragma once
#include "SQLLanguage.h"
#include <sqltypes.h>
#include <iostream>
#include <set>

class SQLDate;
class SQLTime;
class SQLTimestamp;
class SQLDatabase;

// Interval is stored internally as a total number of seconds
typedef __int64 InterValue;

class SQLInterval
{
public:
  SQLInterval();
  SQLInterval(SQLINTERVAL p_type,const CString p_string);
  SQLInterval(SQLInterval& p_interval);
  SQLInterval(SQL_INTERVAL_STRUCT* p_interval);
  SQLInterval(double p_databaseDouble);
  SQLInterval(InterValue p_nanoseconds);
  SQLInterval(int p_years,int p_months);
  SQLInterval(int p_days, int p_hours,int p_minutes,int p_seconds,int p_fraction = 0);
 ~SQLInterval();

  // SETTERS

  bool    SetInterval(SQLINTERVAL p_type,const CString p_string);
  bool    SetInterval(InterValue p_nanoseconds);
  bool    SetInterval(SQL_INTERVAL_STRUCT& p_interval);
  void    SetInterval(int p_years,int p_months);  // if year is negative, interval is negative
  void    SetInterval(int p_days,int p_hours,int p_minutes,int p_seconds,int p_fraction = 0); // days negative -> Interval = negative
  void    SetFromDatabaseDouble(double p_number);
  void    SetFractionPart(int p_fraction);
  bool    SetIntervalType(SQLINTERVAL p_type);

  // GETTERS

  int     GetYears() const;
  int     GetMonths() const;
  int     GetDays() const;
  int     GetHours() const;
  int     GetMinutes() const;
  int     GetSeconds() const;
  int     GetFractionPart() const;

  double  AsDatabaseDouble() const;
  CString AsString() const;
  CString AsXMLString() const;
  CString AsSQLString(SQLDatabase* p_database) const;
  void    AsIntervalStruct(SQL_INTERVAL_STRUCT* p_struct) const;

  // CALCULATIONS

  void    AddYears   (int p_years);
  void    AddMonths  (int p_months);
  void    AddDays    (int p_days);
  void    AddHours   (int p_hours);
  void    AddMinutes (int p_minutes);
  void    AddSeconds (int p_seconds);
  void    AddFraction(int p_fraction);

  // OPERATORS

  // Assignment of the interval
  SQLInterval&  operator= (const SQLInterval& p_interval);
  // Adding to temporal types
  SQLDate       operator+ (const SQLDate& p_date) const;
  SQLTime       operator+ (const SQLTime& p_time) const;
  SQLTimestamp  operator+ (const SQLTimestamp& p_stamp) const;
  SQLInterval   operator+ (const SQLInterval& p_interval) const;
  SQLInterval   operator- (const SQLInterval& p_interval) const;
  // Scalar sizing of the interval
  SQLInterval   operator* (int    p_number) const;
  SQLInterval   operator* (double p_number) const;
  SQLInterval   operator/ (int    p_number) const;
  SQLInterval   operator/ (double p_number) const;
  SQLInterval&  operator*=(int    p_number);
  SQLInterval&  operator*=(double p_number);
  SQLInterval&  operator/=(int    p_number);
  SQLInterval&  operator/=(double p_number);
  // Comparison to other intervals
  bool          operator==(const SQLInterval& p_interval) const;
  bool          operator!=(const SQLInterval& p_interval) const;
  bool          operator >(const SQLInterval& p_interval) const;
  bool          operator>=(const SQLInterval& p_interval) const;
  bool          operator <(const SQLInterval& p_interval) const;
  bool          operator<=(const SQLInterval& p_interval) const;

  // NULL Status
  bool IsNull()  const;
  bool Valid();
  void SetNull();

  // Internals
  InterValue  AsValue() const;
  SQLINTERVAL GetIntervalType() const;
  bool        GetIsYearMonthType() const;
  bool        GetIsDaySecondType() const;
  bool        GetIsTimeType() const;
  bool        GetIsNegative() const;

private:
  // Parsing from a string
  bool ParseInterval(SQLINTERVAL p_type,const CString& p_string);
  // Recalculate the total interval value
  void RecalculateValue();
  // Normalise the field values
  void Normalise();
  // Parse value back to m_interval struct
  void RecalculateInterval();

  // The one and only interval
  SQL_INTERVAL_STRUCT m_interval;
  // The total interval
  // YEAR-MONTH -> Number of months!!
  // DAY-SECOND -> Number of nanoseconds
  InterValue m_value;
};
