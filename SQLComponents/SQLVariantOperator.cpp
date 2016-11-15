////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperator.cpp
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   11-11-2016
// Version number:  1.2.0
//
#include "stdafx.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"

SQLConciseType SQLTypeToConciseType(int p_datatype)
{
  switch(p_datatype)
  {
    case SQL_C_CHAR:                      return CT_CHAR;
    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    return CT_SSHORT;
    case SQL_C_USHORT:                    return CT_USHORT;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     return CT_SLONG;
    case SQL_C_ULONG:                     return CT_ULONG;
    case SQL_C_FLOAT:                     return CT_FLOAT;
    case SQL_C_DOUBLE:                    return CT_DOUBLE;
    case SQL_C_BIT:                       return CT_BIT;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  return CT_STINYINT;
    case SQL_C_UTINYINT:                  return CT_UTINYINT;
    case SQL_C_SBIGINT:                   return CT_SBIGINT;
    case SQL_C_UBIGINT:                   return CT_UBIGINT;
    case SQL_C_NUMERIC:                   return CT_NUMERIC;
    case SQL_C_GUID:                      return CT_GUID;
    case SQL_C_BINARY:                    return CT_BINARY;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 return CT_DATE;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 return CT_TIME;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            return CT_TIMESTAMP;
    case SQL_C_INTERVAL_YEAR:             // fall through
    case SQL_C_INTERVAL_MONTH:            // fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    return CT_INTERVAL_YM;
    case SQL_C_INTERVAL_DAY:              // fall through
    case SQL_C_INTERVAL_HOUR:             // fall through
    case SQL_C_INTERVAL_MINUTE:           // fall through
    case SQL_C_INTERVAL_SECOND:           // fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: return CT_INTERVAL_DS;
  }
  return CT_LAST;
}

// Assignment operator for a new SQLVariant
SQLVariant&
SQLVariant::operator=(const SQLVariant& p_origineel)
{
  // Check for assignment to self
  if(this == &p_origineel)
  {
    return *this;
  }

  // Copy the members
  m_datatype        = p_origineel.m_datatype;
  m_sqlDatatype     = p_origineel.m_sqlDatatype;
  m_binaryLength    = p_origineel.m_binaryLength;
  m_binaryPieceSize = p_origineel.m_binaryPieceSize;
  m_columnNumber    = p_origineel.m_columnNumber;
  m_paramType       = p_origineel.m_paramType;
  m_useAtExec       = p_origineel.m_useAtExec;
  m_indicator       = p_origineel.m_indicator;

  // Copy the data
  if(m_datatype == SQL_C_CHAR  || m_datatype == SQL_C_BINARY )
  {
    // Make a new buffer and copy it
    m_data.m_dataBINARY = (unsigned char*) malloc(m_binaryLength + 1);
    memcpy(m_data.m_dataBINARY,p_origineel.m_data.m_dataBINARY,m_binaryLength + 1);
  }
  else
  {
    // Copy the data member
    memcpy(&m_data.m_dataBIT,&p_origineel.m_data.m_dataBIT,sizeof(m_data));
  }
  return *this;
}

