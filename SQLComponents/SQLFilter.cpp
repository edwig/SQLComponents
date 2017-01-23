#include "stdafx.h"
#include "SQLFilter.h"

namespace SQLComponents
{

// XTOR: Creates a new filter
SQLFilter::SQLFilter(CString p_field,SQLOperator p_operator,SQLVariant& p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
          ,m_value(p_value)
{
}

// Getting the SQL Condition
CString
SQLFilter::GetSQLFilter()
{
  // Add the field
  CString sql = m_field + " ";

  // Add the operator
  switch(m_operator)
  {
    case OP_Equal:        sql += "=";       break;
    case OP_NotEqual:     sql += "<>";      break;
    case OP_Greater:      sql += ">";       break;
    case OP_GreaterEqual: sql += ">=";      break;
    case OP_Smaller:      sql += "<";       break;
    case OP_SmallerEqual: sql += "<=";      break;
    case OP_LikeBegin:    sql += "LIKE '";  break;
    case OP_LikeMiddle:   sql += "LIKE '%"; break;
    case OP_IsNULL:       sql += "IS NULL";
                          return sql;
    case OP_IsNotNULL:    sql += "IS NOT NULL";
                          return sql;
    case OP_NOP:          // Fall through
    default:              throw CString("SQLFilter without an operator!");
  }

  // Getting the value as an SQL expression string (with ODBC escapes)
  CString value = m_value.GetAsSQLString();

  // In case of a LIKE search of a character field
  if(m_operator == OP_LikeBegin || m_operator == OP_LikeMiddle)
  {
    // Strip leading and trailing quote
    value = value.Mid(1);
    value = value.Left(value.GetLength() - 1);
    // Add as a LIKE string
    sql += value;
    sql += "%'";
  }
  else
  {
    sql += " ";
    sql += value;
  }

  return sql;
}

}
