////////////////////////////////////////////////////////////////////////
//
// File: BasicSheet.cpp
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
// Last Revision:   04-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicXmlExcel.h"
#include "BasicExcel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace YExcel;

namespace DatabaseUnitTest
{
  TEST_CLASS(BasicSheet)
  {
    TEST_METHOD(BasicXML)
    {
      Logger::WriteMessage("Unit testing XLSX spreadsheet reading");

      BasicXmlExcel excel("..\\UnitTest\\BasicExcel.xlsx");
      Assert::IsTrue(excel.Load());

      Names sheets;
      excel.GetSheetNames(sheets);
      for(auto& name : sheets)
      {
        Logger::WriteMessage("Sheet name: " + name);
      }

      TestXMLWorksheet(excel,1,"First");
      TestXMLWorksheet(excel,2,"Second");
      TestXMLWorksheet(excel,3,"Third");
    }

    void TestXMLWorksheet(BasicXmlExcel& p_excel,int p_sheet,CString p_name)
    {
      Logger::WriteMessage("Testing worksheet: " + p_name);

      // Get worksheet (zero based)
      BasicXmlWorksheet* sheet = p_excel.GetWorksheet(p_sheet - 1);
      Assert::IsNotNull(sheet);
      CString name = sheet->GetSheetName();

      Assert::IsTrue(p_name == name);

      for(int row = 1; row < 4; ++row)
      {
        for(int col = 1; col < 4; ++col)
        {
          CString expect;
          CString value = sheet->GetCellValue(row,col);
          expect.Format("%d%d%d",p_sheet,row,col);

          Logger::WriteMessage(value);
          Assert::IsTrue(expect == value);
        }
      }
    }

    TEST_METHOD(BasicXLS)
    {
      Logger::WriteMessage("Unit testing XLS spreadsheet reading");

      BasicExcel excel("..\\UnitTest\\BasicExcel.xls");
      CString error = excel.GetError();
      Assert::IsTrue(error.IsEmpty());

      TestWorksheet(excel,1,"First");
      TestWorksheet(excel,2,"Second");
      TestWorksheet(excel,3,"Third");
    }

    void TestWorksheet(BasicExcel& p_excel,int p_sheet,CString p_name)
    {
      Logger::WriteMessage("Testing worksheet: " + p_name);

      // Get worksheet (zero based)
      BasicExcelWorksheet* sheet = p_excel.GetWorksheet(p_sheet - 1);
      Assert::IsNotNull(sheet);
      CString name = sheet->GetAnsiSheetName();

      Assert::IsTrue(p_name == name);

      for(int row = 1; row < 4; ++row)
      {
        for(int col = 1; col < 4; ++col)
        {
          char buffer[200];
          CString expect;
          CString value = sheet->CellValue(row - 1,col - 1,buffer,200);
          expect.Format("%d%d%d",p_sheet,row,col);

          Logger::WriteMessage(value);
          Assert::IsTrue(expect == value);
        }
      }

    }
  };
}