////////////////////////////////////////////////////////////////////////
//
// File: SQLFilter.h
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#pragma once
#include "SQLComponents.h"
#include "SQLVariant.h"
#include <vector>

namespace SQLComponents
{

// Operator for a condition filter
typedef enum _sqlOperator
{
   OP_NOP = 0
  ,OP_Equal = 1
  ,OP_NotEqual
  ,OP_Greater
  ,OP_GreaterEqual
  ,OP_Smaller
  ,OP_SmallerEqual
  ,OP_IsNULL
  ,OP_IsNotNULL
  ,OP_LikeBegin
  ,OP_LikeMiddle
  ,OP_IN
  ,OP_Between
  ,OP_Exists
  ,OP_OR               // OR chaining to next filter(s)
}
SQLOperator;

// All ODBC {fn } escaped functions
typedef enum _sqlFunction
{
   FN_NOP = 0
  ,FN_ASCII
  ,FN_BIT_LENGTH
  ,FN_CHAR
  ,FN_CHAR_LENGTH
  ,FN_CHARACTER_LENGTH
  ,FN_CONCAT
  ,FN_DIFFERENCE
  ,FN_INSERT
  ,FN_LCASE
  ,FN_LEFT
  ,FN_LENGTH
  ,FN_LOCATE
  ,FN_LTRIM
  ,FN_OCTET_LENGTH
  ,FN_POSITION
  ,FN_REPEAT
  ,FN_REPLACE
  ,FN_RIGHT
  ,FN_RTRIM
  ,FN_SOUNDEX
  ,FN_SPACE
  ,FN_SUBSTRING
  ,FN_UCASE
  // NUMERIC
  ,FN_ABS
  ,FN_ACOS
  ,FN_ASIN
  ,FN_ATAN
  ,FN_ATAN2
  ,FN_CEILING
  ,FN_COS
  ,FN_COT
  ,FN_DEGREES
  ,FN_EXP
  ,FN_FLOOR
  ,FN_LOG
  ,FN_LOG10
  ,FN_MOD
  ,FN_PI
  ,FN_POWER
  ,FN_RADIANS
  ,FN_RAND
  ,FN_ROUND
  ,FN_SIGN
  ,FN_SIN
  ,FN_SQRT
  ,FN_TAN
  ,FN_TRUNCATE
  // TIME, DATE
  ,FN_CURRENT_DATE
  ,FN_CURRENT_TIME
  ,FN_CURRENT_TIMESTAMP
  ,FN_CURDATE
  ,FN_CURTIME
  ,FN_DAYNAME
  ,FN_DAYOFMONTH
  ,FN_DAYOFWEEK
  ,FN_DAYOFYEAR
  ,FN_EXTRACT
  ,FN_HOUR
  ,FN_MINUTE
  ,FN_SECOND
  ,FN_MONTH
  ,FN_MONTHNAME
  ,FN_NOW
  ,FN_QUARTER
  ,FN_TIMESTAMPADD
  ,FN_TIMESTAMPDIFF
  ,FN_WEEK
  ,FN_YEAR
  // SYSTEM
  ,FN_DATABASE
  ,FN_IFNULL
  ,FN_USER
}
SQLFunction;

// Forward declarations
class SQLRecord;
class SQLQuery;
// Mapping typedefs
typedef std::vector<SQLVariant*> VariantSet;

// Translate a string from the message to an operator
SQLOperator StringToSQLOperator(CString p_oper);
CString     SQLOperatorToString(SQLOperator p_oper);

// THE FILTER CLASS

class SQLFilter
{
public:
  // Construct a filter
  SQLFilter(CString p_field,SQLOperator p_operator);
  SQLFilter(CString p_field,SQLOperator p_operator,SQLVariant* p_value);
  SQLFilter(CString p_field,SQLOperator p_operator,int         p_value);
  SQLFilter(CString p_field,SQLOperator p_operator,CString     p_value);
  SQLFilter(SQLOperator p_operator);
  SQLFilter(SQLFilter* p_other);
  SQLFilter(SQLFilter& p_other);
 ~SQLFilter();

  // Adding extra values for the IN or BETWEEN operators
  void        AddValue(SQLVariant* p_value);
  // Adding an expression as replacement for concrete values
  void        AddExpression(CString p_expression);
  // Negate the filter
  void        Negate();
  // Setting a parenthesis level
  void        SetOpenParenthesis();
  void        SetCloseParenthesis();
  // Setting an extra function
  void        SetFunction(SQLFunction p_function);

  // OPERATIONS

  // Getting the SQL Condition filter
  CString     GetSQLFilter(SQLQuery& p_query);
  // Match a record to the filter internally
  bool        MatchRecord(SQLRecord* p_record);

  // GETTERS

  // Getting the elements of the filter
  CString     GetField();
  SQLOperator GetOperator();
  SQLVariant* GetValue();
  SQLVariant* GetValue(int p_number);
  CString     GetExpression();
  SQLFunction GetFunction();
  bool        GetNegate();

  SQLFilter&  operator=(const SQLFilter& p_other);
private:
  // Check that we have at least one operand value
  void        CheckValue();
  void        CheckTwoValues();
  // Constructing the default operand
  void        ConstructOperand(CString& p_sql,SQLQuery& p_query);
  // Constructing the LIKE clause
  void        ConstructLike(CString& p_sql);
  // Constructing the IN clause
  void        ConstructIN(CString& p_sql,SQLQuery& p_query);
  // Constructing the BETWEEN clause
  void        ConstructBetween(CString& p_sql,SQLQuery& p_query);
  // Constructing a FUNCTION(field)
  CString     ConstructFunctionSQL(SQLQuery& p_query);

  // Internal matching
  bool        MatchEqual       (SQLVariant* p_field);
  bool        MatchNotEqual    (SQLVariant* p_field);
  bool        MatchGreater     (SQLVariant* p_field);
  bool        MatchGreaterEqual(SQLVariant* p_field);
  bool        MatchSmaller     (SQLVariant* p_field);
  bool        MatchSmallerEqual(SQLVariant* p_field);
  bool        MatchLikeBegin   (SQLVariant* p_field);
  bool        MatchLikeMiddle  (SQLVariant* p_field);
  bool        MatchIsNULL      (SQLVariant* p_field);
  bool        MatchIsNotNull   (SQLVariant* p_field);
  bool        MatchIN          (SQLVariant* p_field);
  bool        MatchBetween     (SQLVariant* p_field);

  // Data for the filter
  CString     m_field;                         // Name of the field to act upon
  SQLOperator m_operator;                      // Operator of the condition
  VariantSet  m_values;                        // Multiple values for IN and BETWEEN
  CString     m_expression;                    // Free expression (no values!)
  SQLFunction m_function         { FN_NOP };   // Extra function for the field operator
  bool        m_negate           { false  };   // Negate the whole condition
  bool        m_openParenthesis  { false  };   // Start with an opening parenthesis
  bool        m_closeParenthesis { false  };   // End with a closing parenthesis
};

inline CString
SQLFilter::GetField()
{
  return m_field;
}

inline SQLOperator
SQLFilter::GetOperator()
{
  return m_operator;
}

inline SQLVariant*
SQLFilter::GetValue()
{
  return m_values.front();
}

inline void
SQLFilter::AddValue(SQLVariant* p_value)
{
  SQLVariant* value = new SQLVariant(p_value);
  m_values.push_back(value);
}

inline void
SQLFilter::AddExpression(CString p_expression)
{
  m_expression = p_expression;
}

inline void
SQLFilter::Negate()
{
  m_negate = !m_negate;
}

inline CString
SQLFilter::GetExpression()
{
  return m_expression;
}

inline bool
SQLFilter::GetNegate()
{
  return m_negate;
}

inline void
SQLFilter::SetOpenParenthesis()
{
  m_openParenthesis = true;
}

inline void
SQLFilter::SetCloseParenthesis()
{
  m_closeParenthesis = true;
}

inline void
SQLFilter::SetFunction(SQLFunction p_function)
{
  m_function = p_function;
}

inline SQLFunction
SQLFilter::GetFunction()
{
  return m_function;
}

//////////////////////////////////////////////////////////////////////////
// And finally: a filter set is a vector of SQLFilters

// using SQLFilterSet = std::vector<SQLFilter>;
class SQLFilterSet
{
public:
  ~SQLFilterSet()
  {
    for(auto& filter : m_filters)
    {
      delete filter;
    }
  }

  void AddFilter(SQLFilter* p_filter)
  {
    SQLFilter* filter = new SQLFilter(p_filter);
    m_filters.push_back(filter);
  }

  void AddFilter(SQLFilter& p_filter)
  {
    SQLFilter* filter = new SQLFilter(p_filter);
    m_filters.push_back(filter);
  }

  bool Empty()
  {
    return m_filters.empty();
  }

  std::vector<SQLFilter*>& GetFilters()
  {
    return m_filters;
  }

  int Size()
  {
    return (int)m_filters.size();
  }

  CString ParseFiltersToCondition(SQLQuery& p_query);

private:
  std::vector<SQLFilter*> m_filters;
};

//////////////////////////////////////////////////////////////////////////
// Alias for other systems

typedef SQLFilter Filter;


}