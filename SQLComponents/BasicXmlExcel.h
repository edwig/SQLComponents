#pragma once
#include "unzip.h"
#include "xml.h"
#include <vector>
#include <map>

// Macro's for finding the cell's row/col
#define CELLNUM(row,col)  (((col) << 20) | (row))
#define ROWNUM(cellnum)   (cellnum & 0xFFFFF)
#define COLNUM(cellnum)   (cellnum >> 20)

// Forward definition
class BasicXmlExcel;

// Type of cells
typedef enum _celltype
{
  CT_EMPTY
 ,CT_INTEGER
 ,CT_DOUBLE
 ,CT_STRING
 ,CT_FORMULA
}
CellType;

class BasicXmlCell
{
public:
  // new empty cell
  BasicXmlCell(int p_row,int p_col);                    
  // new integer cell or string cell
  BasicXmlCell(int p_row,int p_col,int p_value,CellType p_type);
  // new double cell
  BasicXmlCell(int p_row,int p_col,double p_value);   
  // DTOR
 ~BasicXmlCell();

  // Getters
  CellType GetCellType();
  bool     GetCellValue (int&    p_value);
  bool     GetCellValue (double& p_value);
  bool     GetCellString(int&    p_value);
private:
  int       m_row;
  int       m_col;
  CellType  m_type;
  union _val
  {
    int      m_intval;
    double   m_number;
    int      m_string;  // Shared string number
  }
  m_val;
};

typedef unsigned long                 ulong;
typedef std::map<ulong,BasicXmlCell*> XmlCells;
typedef std::vector<CString>          Names;

class BasicXmlWorksheet
{
public:
  BasicXmlWorksheet(BasicXmlExcel* p_workbook,CString p_sheetname);
 ~BasicXmlWorksheet();

  // OPERATIONS
  void          Load(XmlElement* p_root);

  // GETTERS
  CString       GetSheetName();
  int           GetMaxRows();
  int           GetMaxCols();
  BasicXmlCell* GetCell     (int p_row,int p_col);
  CString       GetCellValue(int p_row,int p_col);
private:
  int           CalculateColumnNumber(CString p_column,CString p_row);

  BasicXmlExcel*  m_workbook;
  CString         m_sheetname;
  int             m_maxRow;
  int             m_maxCol;
  // Storage of the sheet
  XmlCells        m_cells;
};

typedef std::vector<BasicXmlWorksheet*> WorkSheets;

class BasicXmlExcel
{
public:
  BasicXmlExcel(CString p_filename);
 ~BasicXmlExcel();

  // OPERATIONS
  bool Load();

  // SETTERS
  void SetError(ZRESULT p_result);
  void SetError(CString p_error);

  // GETTERS
  bool                GetSheetNames(Names& p_names);
  BasicXmlWorksheet*  GetWorksheet(int p_index);
  BasicXmlWorksheet*  GetWorksheet(CString p_name);
  CString             GetSharedString(int p_string);
  CString             GetError();
  
private:
  // Read in the worksheetnames
  void       ReadSheetNames();
  bool       LoadStrings();
  bool       LoadWorksheets();

  CString    m_filename;      // Filename
  HZIP       m_zip;           // ZIP handle for unzipping
  Names      m_sheetnames;
  Names      m_sharedStrings;
  WorkSheets m_worksheets;
  // status
  bool       m_namesRead;
  bool       m_sheetRead;
  CString    m_error;
};
