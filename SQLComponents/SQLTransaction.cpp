////////////////////////////////////////////////////////////////////////
//
// File: SQLTransaction.cpp
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
#include "stdafx.h"
#include "SQLTransaction.h"
#include "SQLDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SQLTransaction::SQLTransaction(SQLDatabase* p_database
                              ,CString      p_name
                              ,bool         p_startImmediate
                              ,bool         p_isSubTransaction) 
               :m_database  (p_database)
               ,m_active    (false)
{
  // If asked for, start it right away
  if(p_startImmediate)
  {
    Start(p_name, p_isSubTransaction);
  }
}

SQLTransaction::~SQLTransaction()
{
  // If still active, rollback the transaction
  if(m_active)
  {
    // Cannot throw in a destructor
    try
    {
      Rollback();
    }
    catch(CString& s)
    {
      // TODO Do the logging
      s;
    }
  }
}

void 
SQLTransaction::Start(CString p_name, bool p_startSubtransaction)
{
  // On transaction per instance
  if(m_active)
  {
    throw CString("Error in start-transaction: Already started a transaction");
  }

  // Try to start the transaction
  m_savepoint = m_database->StartTransaction(this, p_startSubtransaction);

  // We are alive!
  m_name   = p_name;
  m_active = true;
}

void 
SQLTransaction::Commit()
{
  // Error if there is no active transaction
  if(!m_active)
  {
    throw CString("Error in commit: transaction object is not opened");
  }

  // We are no longer started/active, so we do nothing else after destruction
  // so commit's are not tried double on the database
  // NOTE: Savepoints must remain till after the commits for the databagse
  m_active = false;
  
  // Do the commit, if it fails, the database will 
  // automatically do a rollback
  m_database->CommitTransaction(this);

  // Cleanup after use
  m_name      = "";
  m_savepoint = "";
}

void 
SQLTransaction::Rollback()
{
  // Do the rollboack. Cleaning will be done by
  // the AfterRollback method, called by SQLDatabase
  m_database->RollbackTransaction(this);
}

void 
SQLTransaction::AfterRollback()
{
  // After closing the transaction by a rollback
  m_active    = false;
  m_name      = "";
  m_savepoint = "";
}
