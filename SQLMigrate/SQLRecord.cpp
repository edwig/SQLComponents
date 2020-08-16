////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Tools/ExportDB_20/ExportDB_20/SQLRecord.cpp $
// Auteurs:      W.E. Huisman
//
// Copyright (c) 1995 - 2010 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 22-11-10 $
// Gewijzigd door:    $Author: Ehuisman $
// Versienummer:      $Revision: 2 $
//
#include "stdafx.h"
#include "SQLRecord.h"
#include "SQLDataSet.h"

SQLRecord::SQLRecord(SQLDataSet* p_set,bool p_modifiable)
          :m_dataSet(p_set)
          ,m_modifiable(p_modifiable)
          ,m_status(SQL_Record_NULL)
{
}

SQLRecord::~SQLRecord()
{
  for(unsigned int ind = 0;ind < m_orgFields.size();++ind)
  {
    SQLVariant* field = m_orgFields[ind];
    delete field;
  }
  for(unsigned int ind = 0; ind < m_fields.size(); ++ind)
  {
    SQLVariant* field = m_fields[ind];
    delete field;
  }
  m_orgFields.clear();
  m_fields.clear();
  m_status = SQL_Record_NULL;
}

// Set this record's status to a deleted record
void
SQLRecord::Delete()
{
  m_status |= SQL_Record_Deleted;
  m_dataSet->SetStatus(SQL_Deletions);
}

void
SQLRecord::AddField(SQLVariant* p_field
                   ,bool        p_insert /*= false*/)
{
  m_fields.push_back(p_field);
  if(m_modifiable)
  {
    m_orgFields.push_back(p_field);
  }
  // Optioneel kan het een nieuw toegevoegd record zijn.
  if(p_insert)
  {
    m_status |= SQL_Record_Insert;
    m_dataSet->SetStatus(SQL_Insertions);
  }
}

void
SQLRecord::SetField(int p_num,SQLVariant* p_field)
{
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    SQLVariant* var = m_fields[p_num];
    delete var;
    m_fields[p_num] = p_field;
  }
}

SQLVariant*     
SQLRecord::GetField(int p_num)
{
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    return m_fields[p_num];
  }
  return NULL;
}

// Rollback van alle wijzigingen
void
SQLRecord::Rollback()
{
  if(m_modifiable == false)
  {
    return;
  }
  for(unsigned int ind = 0;
      ind < m_orgFields.size(),
      ind < m_fields.size();
      ++ind)
  {
    SQLVariant* field = m_orgFields[ind];
    SQLVariant* copy  = m_fields[ind];
    delete copy;
    copy = new SQLVariant();
    // Gebruik assignment operator van SQLVariant
    *copy = *field;
  }
  // Revert to inserted or selected
  m_status &= ~SQL_Record_Updated;
  m_status &= ~SQL_Record_Deleted;
}

void
SQLRecord::ModifyField(int p_num,void* p_data)
{
  if(m_modifiable == false)
  {
    return;
  }
  if(p_num < 0 || p_num >= (int)m_fields.size())
  {
    return;
  }
  SQLVariant* field = m_fields[p_num];
  if(memcmp(field->GetDataPointer(),p_data,field->GetDataSize()))
  {
    m_status |= SQL_Record_Updated;
  }
  field->SetData(field->GetDataType(),(char*)p_data);
  m_dataSet->SetStatus(SQL_Updates);
}


//////////////////////////////////////////////////////////////////////////
//
// XBDF Opslaan en teruglezen
//
//////////////////////////////////////////////////////////////////////////

void
SQLRecord::Opslaan(XmlElement* p_records)
{
  XmlElement* record = new XmlElement("Record");
  p_records->LinkEndChild(record);

  for(unsigned int ind = 0;ind < m_fields.size(); ++ind)
  {
    SQLVariant* var = m_fields[ind];
    CString naam = m_dataSet->GetFieldName(ind);
    int type = var->GetDataType();

    XmlElement* veld = new XmlElement("Veld");
    record->LinkEndChild(veld);
    veld->SetAttribute("id",ind);
    veld->SetAttribute("type",type);
    veld->SetAttribute("naam",naam);

    CString val;
    var->GetAsString(val);
    XmlText* waarde = new XmlText(val);
    veld->LinkEndChild(waarde);
  }
}

void
SQLRecord::Laden(XmlElement* p_record)
{
  int ind = 0;
  XmlElement* veld = p_record->FirstChildElement();
  while(veld)
  {
    int type = 0;
    int id   = 0;
    veld->Attribute("type",&type);
    veld->Attribute("id",&id);
    if(ind != id)
    {
      throw CString("Ongeldige veld sequence in record of: ") + m_dataSet->GetName();
    }
    SQLVariant* var = new SQLVariant();
    char* text = (char*)veld->GetText();
    if(!text) 
    {
      // Lege nodes leveren een NULL Pointer op!
      text = "";
    }
    var->SetData(type,text);
    AddField(var);

    // Volgende veld
    veld = veld->NextSiblingElement();
    ++ind;

  }
}
