#pragma once
#include "SQLDatabase.h"
#include "SQLRecord.h"
#include "SQLVariant.h"
#include <vector>

// SET status is a OR-ed bitmap status
#define SQL_Empty       0x00
#define SQL_Selections  0x01
#define SQL_Updates     0x02
#define SQL_Deletions   0x04
#define SQL_Insertions  0x08

// Parameter voor de query
typedef struct _sql_parameter
{
  CString    m_naam;
  SQLVariant m_waarde;
}
SQLParameter;

class AggregateInfo
{
public:
  void    Init();

  double  m_sum;
  double  m_max;
  double  m_min;
  double  m_mean;
};

class SQLDatabase;
class SQLQuery;
typedef std::vector<SQLRecord*>   RecordSet;
typedef std::vector<SQLParameter> ParameterSet;
typedef std::vector<CString>      NamenMap;
typedef std::vector<int>          TypenMap;

class SQLDataSet
{
public:
  SQLDataSet(CString p_name,SQLDatabase* p_database = NULL);
 ~SQLDataSet();

  // Query done and records gotten?
  bool IsOpen();
  // Perform query: Do SetQuery first
  bool Open();
  // Remove resultset
  void Close();

  // Navigate in the records
  void First();
  int  Next();
  int  Prev();
  int  Last();
  void Goto(int p_record);
  bool IsFirst();
  bool IsLast();

  // Set database connection
  void SetDatabase(SQLDatabase* p_database);
  // Set a new query
  void SetQuery(CString p_query);
  // Set parameter for a query
  void SetParameter(SQLParameter p_parameter);
  void SetParameter(CString p_naam,SQLVariant p_waarde);
  // Set the status to modified/saved
  void SetStatus(int m_add,int m_delete = 0);

  // Insert new record
  SQLRecord* InsertRecord();
  // Insert new field in new record
  int        InsertField(CString& p_name,SQLVariant* p_value);
  // Set a field value in the current record
  void       SetField(int p_num,SQLVariant* p_value);
  // Set a field value in the current record
  void       SetField(CString& p_name,SQLVariant* p_value);
  // Calculate aggregate functions
  int        Aggregate(int p_num,AggregateInfo& p_info);


  // Get the name of the dataset
  CString    GetName();
  // Get the number of records
  int        GetNumberOfRecords();
  // Get number of fields
  int        GetNumberOfFields();
  // Get the current record number
  int        GetCurrentRecord();
  // Get a specific record
  SQLRecord* GetRecord(int p_recnum);
  // Gets the status of records of the dataset
  int        GetStatus();
  // Get a fieldname
  CString    GetFieldName(int p_num);
  // Get datatype of a field
  int        GetFieldType(int p_num);
  // Get a field number
  int        GetFieldNumber(CString& p_name);
  // Get a fiedl from the current record
  SQLVariant* GetCurrentField(int p_num);

  // XBDF Opslaan en Inlezen
  void       Opslaan(XmlElement* p_dataset);
  void       Laden  (XmlElement* p_dataset);
private:
  // Set parameters in the query
  CString      ParseQuery();
  // Get the variant of a parameter
  SQLVariant*  GetParameter(CString& p_name);
  // Get all the columns of the record
  void         ReadNamen(SQLQuery& qr);
  // Get all the datatypes of the columns
  void         ReadTypen(SQLQuery& qr);

  CString      m_name;
  SQLDatabase* m_database;
  bool         m_open;
  CString      m_query;
  ParameterSet m_parameters;
  NamenMap     m_namen;
  TypenMap     m_typen;
  RecordSet    m_records;
  int          m_status;
  int          m_current;
};

inline void 
SQLDataSet::SetDatabase(SQLDatabase* p_database)
{
  m_database = p_database;
}

inline bool
SQLDataSet::IsOpen()
{
  return m_open;
}

inline void 
SQLDataSet::SetQuery(CString p_query)
{
  m_query = p_query;
}

inline int
SQLDataSet::GetNumberOfRecords()
{
  return m_records.size();
}

inline int
SQLDataSet::GetNumberOfFields()
{
  return m_namen.size();
}

inline int
SQLDataSet::GetCurrentRecord()
{
  return m_current;
}

inline int
SQLDataSet::GetStatus()
{
  return m_status;
}

inline CString    
SQLDataSet::GetName()
{
  return m_name;
}

inline void 
AggregateInfo::Init()
{
  m_sum  = 0.0;
  m_max  = 0.0;
  m_min  = 0.0;
  m_mean = 0.0;
}

