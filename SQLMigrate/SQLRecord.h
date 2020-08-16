////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLRecord.h $
// Auteurs:      W.E. Huisman
//
// Copyright (c) 1995 - 2010 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 25-05-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 1 $
//
#pragma once
#include "SQLVariant.h"
#include "xml.h"
#include <vector>

// Status van het record

#define SQL_Record_NULL      0x00
#define SQL_Record_Selected  0x01
#define SQL_Record_Updated   0x02
#define SQL_Record_Deleted   0x04
#define SQL_Record_Insert    0x08

typedef std::vector<SQLVariant*> FieldMap;

class SQLDataSet;

class SQLRecord
{
public:
  SQLRecord(SQLDataSet* p_set,bool p_modifiable = false);
 ~SQLRecord();
  // Get the status of the record
  int         GetStatus();
  SQLVariant* GetField(int p_num);
  void        AddField(SQLVariant* p_field,bool p_insert = false);
  void        SetField(int p_num,SQLVariant* p_field);
  void        Rollback();
  void        ModifyField(int p_num,void* p_data);
  void        Delete();

  // XBDF opslaan en teruglezen
  void        Opslaan(XmlElement* p_records);
  void        Laden  (XmlElement* p_record);
private:
  SQLDataSet* m_dataSet;
  bool        m_modifiable;
  int         m_status;
  FieldMap    m_orgFields;
  FieldMap    m_fields;
};

inline int
SQLRecord::GetStatus()
{
  return m_status;
}



