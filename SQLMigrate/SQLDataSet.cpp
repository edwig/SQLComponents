#include "stdafx.h"
#include "SQLDataSet.h"
#include "SQLQuery.h"
#include "SQLVariantFormat.h"
#include "xml.h"

SQLDataSet::SQLDataSet(CString p_name,SQLDatabase* p_database /*=NULL*/)
           :m_name(p_name)
           ,m_database(p_database)
           ,m_status(SQL_Empty)
           ,m_current(-1)
{
}

SQLDataSet::~SQLDataSet()
{
  Close();
}

// Navigate in the records
void 
SQLDataSet::First()
{
  m_current = (m_records.size() > 0) ? 0 : -1;
}

int  
SQLDataSet::Next()
{
  if(m_current < (int)m_records.size() - 1)
  {
    return ++m_current;
  }
  return -1;
}

int  
SQLDataSet::Prev()
{
  if(m_current > 0)
  {
    --m_current;
  }
  return m_current;
}

int  
SQLDataSet::Last()
{
  m_current = (m_records.size() > 0) ? m_records.size() - 1 : -1;
  return m_current;
}

void
SQLDataSet::Goto(int p_record)
{
  if(p_record >= 0 && p_record < (int)m_records.size())
  {
    m_current = p_record;
  }
}

// Eerste of lege dataset
bool
SQLDataSet::IsFirst()
{
  return (m_current < 1);
}

// Laatste of lege dataset
bool
SQLDataSet::IsLast()
{
  return m_current == (int)(m_records.size() - 1);
}

void
SQLDataSet::Close()
{
  for(unsigned int ind = 0; ind < m_records.size(); ++ind)
  {
    SQLRecord* record = m_records[ind];
    delete record;
  }
  m_namen.clear();
  m_records.clear();
  m_parameters.clear();
  m_open = false;
}

void
SQLDataSet::SetStatus(int m_add,int m_delete /*=0*/)
{
  m_status |= m_add;
  if(m_delete)
  {
    m_status &= ~m_delete;
  }
}

void 
SQLDataSet::SetParameter(SQLParameter p_parameter)
{
  // Zoek of het een al bestaande is
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_naam == p_parameter.m_naam)
    {
      // Gevonden: Zet nieuwe waarde
      m_parameters[ind].m_waarde = p_parameter.m_waarde;
      return;
    }
  }
  // Nieuwe parameter
  m_parameters.push_back(p_parameter);
}

void 
SQLDataSet::SetParameter(CString p_naam,SQLVariant p_waarde)
{
  // Zoek of het een bestaande parameter is
  for(unsigned int ind = 0;ind < m_parameters.size();++ind)
  {
    if(m_parameters[ind].m_naam == p_naam)
    {
      // Gevonden: zet nieuwe waarde
      m_parameters[ind].m_waarde = p_waarde;
      return;
    }
  }
  // Maak nieuwe parameter
  SQLParameter* par = new SQLParameter();
  par->m_naam   = p_naam;
  par->m_waarde = p_waarde;
  m_parameters.push_back(*par);
}

SQLVariant*  
SQLDataSet::GetParameter(CString& p_name)
{
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_naam == p_name)
    {
      return &(m_parameters[ind].m_waarde);
    }
  }
  return NULL;
}

// Vervang $naam voor de waarde van de parameter
// $ tekens binnen 'string$reeks' of "String$reeks" worden NIET vervangen
// Dit maakt het mogelijk om queries zo te schrijven:
// SELECT t.naam
//       ,s.getal
//       ,'Constante met een $ erin'
//   FROM tabel t
//       ,$parameter1 s
//  WHERE t.id   =    $parameter2
//    AND t.naam LIKE $parameter3
// ;
CString
SQLDataSet::ParseQuery()
{
  bool inQuote = false;
  bool inAphos = false;
  CString query;
  int  pos = 0;

  while(pos < m_query.GetLength())
  {
    int c = m_query.GetAt(pos);
    if(c == '\'')
    {
      inAphos = !inAphos;
    }
    else if(c == '\"')
    {
      inQuote = !inQuote;
    }
    if(c == '$' && !inAphos && !inQuote)
    {
      CString parNaam;
      do 
      {
        c = m_query.GetAt(++pos);
        parNaam += (char) c;
      } 
      while (isalnum(c) || c == '_');
      parNaam = parNaam.Left(parNaam.GetLength() - 1);
      // naam gevonden
      SQLVariant* par = GetParameter(parNaam);
      if(par)
      {
        // TODO: Moet nog door de INFO klasse heen
//         SQLVariantFormat form(par);
//         query += form.FormatVariantForSQL(m_database->GeefDatabaseType());
      }
    }
    else
    {
      query += (char)c;
      ++pos;
    }
  }
  return query;
}

bool
SQLDataSet::Open()
{
  bool   result     = false;
  bool   readNamen  = true;
  CString query     = m_query;

  if(m_query.IsEmpty())
  {
    return false;
  }
  if(m_open)
  {
    Close();
  }
  // If parameters, parse them
  if(m_parameters.size())
  {
    query = ParseQuery();
  }
  try
  {
    CString name = "Open SQL dataset";
    SQLQuery qr(m_database);
    qr.DoSQLStatement(query,name);
    while(qr.GetRecord())
    {
      // Make a new record
      SQLRecord* record = new SQLRecord(this);
      m_records.push_back(record);
      // First pass: read the names of the fields
      if(readNamen)
      {
        ReadNamen(qr);
        ReadTypen(qr);
        readNamen = false;
      }
      // Get all the columns of the record
      SQLVariant* var = NULL;
      int num = qr.GetNumberOfColumns();
      for(int ind = 1; ind <= num; ++ind)
      {
        var = qr.GetColumn(ind);
        SQLVariant* copy = new SQLVariant();
        *copy = *var;
        record->AddField(copy);
      }
    }
    m_open = true;
    result = true;
  }
  catch(CString s)
  {
    Close();
    throw s;
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return result;
}

// Get all the columns of the record
void
SQLDataSet::ReadNamen(SQLQuery& qr)
{
  CString naam;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    qr.GetColumnName(ind,naam);
    m_namen.push_back(naam);
  }
}

void
SQLDataSet::ReadTypen(SQLQuery& qr)
{
  int type;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    type = qr.GetColumnType(ind);
    m_typen.push_back(type);
  }
}

SQLRecord*
SQLDataSet::GetRecord(int p_recnum)
{
  if((unsigned)p_recnum >= 0 && (unsigned)p_recnum < m_records.size())
  {
    return m_records[p_recnum];
  }
  return NULL;
}


// Get a fieldname
CString    
SQLDataSet::GetFieldName(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_namen.size())
  {
    return m_namen[p_num];
  }
  return "";
}

int
SQLDataSet::GetFieldType(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_typen.size())
  {
    return m_typen[p_num];
  }
  return -1;
}

// Get a field number
int
SQLDataSet::GetFieldNumber(CString& p_name)
{
  for(unsigned int ind = 0; ind < m_namen.size(); ++ind)
  {
    if(p_name.CompareNoCase(m_namen[ind]) == 0)
    {
      return ind;
    }
  }
  return -1;
}

SQLVariant*
SQLDataSet::GetCurrentField(int p_num)
{
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    if(p_num >= 0 && p_num < (int)m_namen.size())
    {
      return m_records[m_current]->GetField(p_num);
    }
  }
  return NULL;
}

// Insert new record
SQLRecord* 
SQLDataSet::InsertRecord()
{
  SQLRecord* record = new SQLRecord(this);
  m_records.push_back(record);
  m_current = m_records.size() - 1;

  return record;
}

// Insert new field in new record
int
SQLDataSet::InsertField(CString& p_name,SQLVariant* p_value)
{
  m_namen.push_back(p_name);
  m_typen.push_back(p_value->GetDataType());
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    SQLRecord* record = m_records[m_current];
    record->AddField(p_value);
    return OK;
  }
  // Internal programming error
  return BRIEF_ERROR;
}

// Set a field value in the current record
void
SQLDataSet::SetField(CString& p_name,SQLVariant* p_value)
{
  int num = GetFieldNumber(p_name);
  if(num >= 0)
  {
    SetField(num,p_value);
  }
}

// Set a field value in the current record
void       
SQLDataSet::SetField(int p_num,SQLVariant* p_value)
{
  if(m_current >= 0)
  {
    m_records[m_current]->SetField(p_num,p_value);
  }
}

// Calculate aggregate functions
int
SQLDataSet::Aggregate(int p_num,AggregateInfo& p_info)
{
  unsigned int total = m_records.size();
  for(unsigned int ind = 0;ind < total; ++ind)
  {
    SQLVariant* var = m_records[ind]->GetField(p_num);
    if(var)
    {
      double waarde = var->GetAsDouble();
      if(p_info.m_max < waarde)
      {
        p_info.m_max = waarde;
      }
      if(p_info.m_min > waarde)
      {
        p_info.m_min = waarde;
      }
      p_info.m_sum += waarde;
      p_info.m_mean = p_info.m_sum / total;
    }
  }
  return OK;
}

//////////////////////////////////////////////////////////////////////////
//
// Opslaan in XBDF formaat
//
//////////////////////////////////////////////////////////////////////////

void
SQLDataSet::Opslaan(XmlElement* p_dataset)
{
  // Naam van de dataset
  XmlElement* naam = new XmlElement("Naam");
  XmlText* ntext = new XmlText(m_name);
  p_dataset->LinkEndChild(naam);
  naam->LinkEndChild(ntext);

  // Veldenstructuur van de dataset
  XmlElement* structur = new XmlElement("Structuur");
  p_dataset->LinkEndChild(structur);

  SQLRecord* record = GetRecord(0);
  if(record)
  {
    for(unsigned int ind = 0; ind < m_namen.size(); ++ind)
    {
      CString naam = GetFieldName(ind);
      SQLVariant* var = record->GetField(ind);
      int type = var->GetDataType();
      
      XmlElement* veld = new XmlElement("Veld");
      structur->LinkEndChild(veld);
      veld->SetAttribute("id",ind);
      veld->SetAttribute("type",type);
      veld->SetAttribute("typenaam",var->FindDatatype(type));

      XmlText* ntext = new XmlText(naam);
      veld->LinkEndChild(ntext);
    }
  }

  // Alle records van de dataset
  XmlElement* records = new XmlElement("Records");
  p_dataset->LinkEndChild(records);
  for(unsigned int ind = 0; ind < m_records.size(); ++ind)
  {
    SQLRecord* record = m_records[ind];
    record->Opslaan(records);
  }
}

void
SQLDataSet::Laden(XmlElement* p_dataset)
{
  XmlElement* structur = p_dataset->FindElement("Structuur");
  XmlElement* records  = p_dataset->FindElement("Records");
  if(structur == NULL) throw CString("Structuur deel ontbreekt van dataset") + m_name;
  if(records  == NULL) throw CString("Records deel ontbreekt van dataset") + m_name;

  // Structuur inlezen
  XmlElement* veld = structur->FirstChildElement();
  while(veld)
  {
    // Onthoud de naam van het veld
    CString naam = veld->GetText();
    m_namen.push_back(naam);
    // Datatype van het veld
    int type = atoi(veld->Attribute("type"));
    m_typen.push_back(type);
    // Volgende veld
    veld = veld->NextSiblingElement();
  }

  // Records inlezen
  XmlElement* record = records->FirstChildElement();
  while(record)
  {
    // Maak record en laad het
    SQLRecord* rec = InsertRecord();
    rec->Laden(record);
    // Volgende reord
    record = record->NextSiblingElement();
  }
}
