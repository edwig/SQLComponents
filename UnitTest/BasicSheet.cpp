////////////////////////////////////////////////////////////////////////
//
// File: BasicSheet.cpp
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
// Version number: See SQLComponents.h
//
#include "stdafx.h"
#include "BasicXmlExcel.h"
#include "BasicExcel.h"
#include "UnitTest.h"

using namespace YExcel;

namespace DatabaseUnitTest
{
  TEST_CLASS(BasicSheet)
  {
    TEST_METHOD(BasicXML)
    {
      Logger::WriteMessage("Unit testing XLSX spreadsheet reading");
      InitSQLComponents();

      BasicXmlExcel excel(_T("..\\UnitTest\\BasicExcel.xlsx"));
      Assert::IsTrue(excel.Load());
      number_of_tests++;

      Names sheets;
      excel.GetSheetNames(sheets);
      for(auto& name : sheets)
      {
        Logger::WriteMessage("Sheet name: " + name);
        number_of_tests++;
      }

      TestXMLWorksheet(excel,1,_T("First"));
      TestXMLWorksheet(excel,2,_T("Second"));
      TestXMLWorksheet(excel,3,_T("Third"));
    }

    void TestXMLWorksheet(BasicXmlExcel& p_excel,int p_sheet,XString p_name)
    {
      Logger::WriteMessage("Testing worksheet: " + p_name);

      // Get worksheet (zero based)
      BasicXmlWorksheet* sheet = p_excel.GetWorksheet(p_sheet - 1);
      Assert::IsNotNull(sheet);
      XString name = sheet->GetSheetName();
      number_of_tests++;

      Assert::IsTrue(p_name == name);
      number_of_tests++;

      for(int row = 1; row < 4; ++row)
      {
        for(int col = 1; col < 4; ++col)
        {
          XString expect;
          XString value = sheet->GetCellValue(row,col);
          expect.Format(_T("%d%d%d"),p_sheet,row,col);

          Logger::WriteMessage(value);
          Assert::IsTrue(expect == value);
          number_of_tests++;
        }
      }
    }

    TEST_METHOD(BasicXLS)
    {
      Logger::WriteMessage("Unit testing XLS spreadsheet reading");
      InitSQLComponents();

      BasicExcel excel(_T("..\\UnitTest\\BasicExcel.xls"));
      XString error = excel.GetError();
      Assert::IsTrue(error.IsEmpty());
      number_of_tests++;

      TestWorksheet(excel,1,_T("First"));
      TestWorksheet(excel,2,_T("Second"));
      TestWorksheet(excel,3,_T("Third"));
    }

    void TestWorksheet(BasicExcel& p_excel,int p_sheet,XString p_name)
    {
      Logger::WriteMessage("Testing worksheet: " + p_name);

      // Get worksheet (zero based)
      BasicExcelWorksheet* sheet = p_excel.GetWorksheet(p_sheet - 1);
      Assert::IsNotNull(sheet);
      XString name = sheet->GetAnsiSheetName();
      number_of_tests++;

      Assert::IsTrue(p_name == name);

      for(int row = 1; row < 4; ++row)
      {
        for(int col = 1; col < 4; ++col)
        {
          TCHAR buffer[200];
          XString expect;
          XString value = sheet->CellValue(row - 1,col - 1,buffer,200);
          expect.Format(_T("%d%d%d"),p_sheet,row,col);

          Logger::WriteMessage(value);
          Assert::IsTrue(expect == value);
          number_of_tests++;
        }
      }
    }
  };
}