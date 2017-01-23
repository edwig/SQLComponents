#pragma once
#include "SQLComponents.h"
#include "SQLVariant.h"
#include <vector>

namespace SQLComponents
{

typedef enum _sqlOperator
{
  OP_NOP     = 0
 ,OP_Equal   = 1
 ,OP_NotEqual
 ,OP_Greater
 ,OP_GreaterEqual
 ,OP_Smaller
 ,OP_SmallerEqual
 ,OP_IsNULL
 ,OP_IsNotNULL
 ,OP_LikeBegin
 ,OP_LikeMiddle
}
SQLOperator;

class SQLFilter
{
public:
  SQLFilter(CString p_field,SQLOperator p_operator,SQLVariant& p_value);

  // Getting the elements of the filter
  CString     GetField();
  SQLOperator GetOperator();
  SQLVariant& GetValue();
  // Getting the SQL Condition
  CString     GetSQLFilter();

private:
  CString     m_field;
  SQLOperator m_operator;
  SQLVariant  m_value;
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

inline SQLVariant&
SQLFilter::GetValue()
{
  return m_value;
}

//////////////////////////////////////////////////////////////////////////
// And finaly: a filter set is a vector of SQLFilters

using SQLFilterSet = std::vector<SQLFilter>;

}