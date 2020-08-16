////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLWrappers.h $
// Auteurs:      Jan-Jaap Fahner [JF]
//
// Copyright (c) 1998 - 2006 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 25-05-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 1 $
//
#pragma once

// Door deze define worden we een 'standard-compliant' applicatie.
// Aangezien het gedrag van bijna alle SQLXXX functies hierdoor
// verandert, willen we dit niet zomaar aanzetten.
//
// Bekende gedragsveranderingen:
// - SQLAllocXXX geeft een ander soort handles terug
// - SQLDescribeCol geeft andere types terug
//
// #define ODBC_STD
//

// Maak een constante waarmee we de vlag kunnen zien in c++
#ifdef ODBC_STD
enum { odbc_std_app = true  } ;
#else
enum { odbc_std_app = false } ;
#endif

#pragma warning(disable: 4702)

//////////////////////////////////////////////////////////////////////////
//
// Deze file bevat wrappers voor alle SQLXxx functies om
// access violations en dergelijke op te kunnen vangen.
//

inline SQLRETURN SqlDriverConnect(SQLHDBC hdbc, SQLHWND hwnd, SQLCHAR *szConnStrIn, SQLSMALLINT cbConnStrIn, SQLCHAR *szConnStrOut, SQLSMALLINT cbConnStrOutMax, SQLSMALLINT *pcbConnStrOut, SQLUSMALLINT fDriverCompletion)
{
  try
  {
    return ::SQLDriverConnect(hdbc, hwnd, szConnStrIn, cbConnStrIn, szConnStrOut, cbConnStrOutMax, pcbConnStrOut, fDriverCompletion);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlDisconnect(SQLHDBC ConnectionHandle)
{
  try
  {
    return ::SQLDisconnect(ConnectionHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  try
  {
    return SQLFreeHandle(HandleType, Handle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetInfo(SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT* pcbInfoValue)
{
  try
  {
    return SQLGetInfo(hdbc, fInfoType, rgbInfoValue, cbInfoValueMax, pcbInfoValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, SQLHANDLE *OutputHandle)
{
  try
  {
    return SQLAllocHandle(HandleType, InputHandle, OutputHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocEnv(SQLHENV *EnvironmentHandle)
{
  try
  {
    return SQLAllocEnv(EnvironmentHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocConnect(SQLHENV EnvironmentHandle, SQLHDBC *ConnectionHandle)
{
  try
  {
    return SQLAllocConnect(EnvironmentHandle, ConnectionHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocStmt(SQLHDBC ConnectionHandle, SQLHSTMT *StatementHandle)
{
  try
  {
    return SQLAllocStmt(ConnectionHandle, StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetDiagRec(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLCHAR *szSqlState, SQLINTEGER *pfNativeError, SQLCHAR *szErrorMsg, SQLSMALLINT cbErrorMsgMax, SQLSMALLINT *pcbErrorMsg)
{
  try
  {
    return SQLGetDiagRec(fHandleType, handle, iRecord, szSqlState, pfNativeError, szErrorMsg, cbErrorMsgMax, pcbErrorMsg);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType)
{
  try
  {
    return SQLEndTran(HandleType, Handle, CompletionType);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlSetConnectAttr(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValue)
{
  try
  {
    return SQLSetConnectAttr(hdbc, fAttribute, rgbValue, cbValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlCloseCursor(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLCloseCursor(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlSetStmtOption(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLULEN Value)
{
  try
  {
    return SQLSetStmtOption(StatementHandle, Option, Value);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlPrepare(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLPrepare(hstmt, szSqlStr, cbSqlStr);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNumParams(SQLHSTMT hstmt, SQLSMALLINT *pcpar)
{
  try
  {
    return SQLNumParams(hstmt, pcpar);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlExecute(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLExecute(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlExecDirect(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLExecDirect(hstmt, szSqlStr, cbSqlStr);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlRowCount(SQLHSTMT StatementHandle, SQLLEN* RowCount)
{
  try
  {
    return SQLRowCount(StatementHandle, RowCount);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlFetch(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLFetch(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNumResultCols(SQLHSTMT StatementHandle, SQLSMALLINT *ColumnCount)
{
  try
  {
    return SQLNumResultCols(StatementHandle, ColumnCount);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlDescribeCol(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLCHAR *szColName, SQLSMALLINT cbColNameMax, SQLSMALLINT *pcbColName, SQLSMALLINT *pfSqlType, SQLUINTEGER *pcbColDef, SQLSMALLINT *pibScale, SQLSMALLINT *pfNullable)
{
  try
  {
    return SQLDescribeCol(hstmt, icol, szColName, cbColNameMax, pcbColName, pfSqlType, pcbColDef, pibScale, pfNullable);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlBindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, SQLLEN *StrLen_or_Ind)
{
  try
  {
    return ::SQLBindCol(StatementHandle, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlBindParameter(SQLHSTMT hstmt, SQLUSMALLINT ipar, SQLSMALLINT fParamType, SQLSMALLINT fCType, SQLSMALLINT fSqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN *pcbValue)
{
  try
  {
    return ::SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNativeSql(HDBC hdbc,SQLCHAR* stmt,SQLINTEGER len,SQLCHAR* sql,SQLINTEGER max,SQLINTEGER* reslen)
{
  try
  {
    return ::SQLNativeSql(hdbc,stmt,len,sql,max,reslen);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlDataSources(SQLHENV       EnvironmentHandle
                                ,SQLUSMALLINT  Direction
                                ,SQLCHAR*      ServerName
                                ,SQLSMALLINT   BufferLength1
                                ,SQLSMALLINT*  NameLength1Ptr
                                ,SQLCHAR*      Description
                                ,SQLSMALLINT   BufferLength2
                                ,SQLSMALLINT*  NameLength2Ptr)
{
  try
  {
    return SQLDataSources(EnvironmentHandle,Direction,ServerName,BufferLength1,NameLength1Ptr,Description,BufferLength2,NameLength2Ptr);
  }
  catch (...)
  {
    return SQL_ERROR;
  }
}