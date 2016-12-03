#include "stdafx.h"
#include "SQLVariant.h"
#include "SQLDate.h"
#include "bcd.h"

SQLVariant variants[CT_LAST];
SQLVariant others  [CT_LAST];

void TestGreater()
{
  printf("Testing SQLVariant > SQLVariant\n");
  printf("===============================\n");

  SQLVariant var1((short)13);
  SQLVariant var2((long) 15);

  if(var1 > var2)
  {
    printf("Error in operator (short > long)\n");
  }
  else
  {
    printf("OK\n");
  }
}

void TestSub()
{
  printf("Testing SQLDate - SQLDate -> SQLInterval\n");
  printf("========================================\n");

  SQLDate date1("15-10-1959");
  SQLDate date2("20-11-2016");
  SQLVariant var1(&date1);
  SQLVariant var2(&date2);

  SQLVariant var3 = var2 - var1;
  printf("Found interval is: %s\n",var3.GetAsChar());
}

void TestEqual()
{
  printf("Testing the equality operator of SQLVariant\n");
  printf("===========================================\n");

  // Init the test array
  variants[CT_CHAR]     = SQLVariant("42");
  variants[CT_SSHORT]   = SQLVariant((short)42);
  variants[CT_USHORT]   = SQLVariant((unsigned short)42);
  variants[CT_SLONG]    = SQLVariant((long)42);
  variants[CT_ULONG]    = SQLVariant((unsigned long)42);
  variants[CT_FLOAT]    = SQLVariant((float)42.0);
  variants[CT_DOUBLE]   = SQLVariant((double)42.0);
  variants[CT_BIT]      = SQLVariant(true);
  variants[CT_STINYINT] = SQLVariant((char)42);
  variants[CT_UTINYINT] = SQLVariant((unsigned char)42);
  variants[CT_SBIGINT]  = SQLVariant((__int64)42);
  variants[CT_UBIGINT]  = SQLVariant((unsigned __int64)42);

  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    for(int y = CT_CHAR; y <= CT_UBIGINT; ++y)
    {
      SQLVariant* left  = &variants[x];
      SQLVariant* right = &variants[y];

      CString leftType  = left ->FindDatatype(left ->GetDataType());
      CString rightType = right->FindDatatype(right->GetDataType());
      CString message;
      message.Format("Equality test %s == %s ",leftType,rightType);
      printf(message);

      printf(*left == *right ? "OK\n" : "ERROR\n");
    }
  }
}

void
TestAddVar()
{
  
}

void
TestAdd()
{
  printf("Testing the add operator of SQLVariant\n");
  printf("======================================\n");

  // Init the test array
  others[CT_CHAR]     = SQLVariant("5");
  others[CT_SSHORT]   = SQLVariant((short)5);
  others[CT_USHORT]   = SQLVariant((unsigned short)5);
  others[CT_SLONG]    = SQLVariant((long)5);
  others[CT_ULONG]    = SQLVariant((unsigned long)5);
  others[CT_FLOAT]    = SQLVariant((float)5);
  others[CT_DOUBLE]   = SQLVariant((double)5);
  others[CT_BIT]      = SQLVariant(true);
  others[CT_STINYINT] = SQLVariant((char)5);
  others[CT_UTINYINT] = SQLVariant((unsigned char)5);
  others[CT_SBIGINT]  = SQLVariant((__int64)5);
  others[CT_UBIGINT]  = SQLVariant((unsigned __int64)5);

  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    for(int y = CT_CHAR; y <= CT_UBIGINT; ++y)
    {
      if(x == CT_BIT || y == CT_BIT)
      {
        continue;
      }

      SQLVariant* left  = &variants[x];
      SQLVariant* right = &others[y];

      CString leftType  = left ->FindDatatype(left ->GetDataType());
      CString rightType = right->FindDatatype(right->GetDataType());
      CString message;
      message.Format("Add test %s + %s ",leftType,rightType);
      printf(message);

      SQLVariant result(*left + *right); // TEST
      CString res;
      result.GetAsString(res);

      if(x == CT_CHAR && y == CT_CHAR)
      {
        // Adding strings
        printf(res.Compare("425") ? "ERROR\n" : "OK\n");
      }
      else
      {
        printf(res.Compare("47") ? "ERROR\n" : "OK\n");
      }
    }
  }
}

void TestMul()
{
  CString numString("42");
  bcd num(5);

  SQLVariant var1(numString);
  SQLVariant var2(&num,8,2);

  SQLVariant var3 = var1 * var2;
  printf("The result of the char*decimal = %s\n",var3.GetAsChar());
}

void TestCast()
{
  printf("Testing the cast operators of SQLVariant\n");
  printf("========================================\n");

  SQLVariant varBit   ((bool) true);
  SQLVariant varSTiny ((char) -67);
  SQLVariant varUTiny ((unsigned char) 189);
  SQLVariant varSShort((short) -567);
  SQLVariant varUShort((ushort) 567);
  SQLVariant varInt   ((long) 2 * 42);
  SQLVariant varUInt  ((ulong) 0x84556677);
  
  bool     numberBit     = varBit;
  char     numberSTiny   = varSTiny;
  uchar    numberUTiny   = varUTiny;
  short    numberSShort  = varSShort;
  ushort   numberUShort  = varUShort;
  int      numberInt     = varInt;
  unsigned numberUInt    = varUInt;

  printf("Test for correct bit cast               : %s\n",numberBit ? "true" : "false");
  printf("Test for correct signed tiny int cast   : %d\n",(int) numberSTiny);
  printf("Test for correct unsigned tiny int cast : %d\n",(int) numberUTiny);
  printf("Test for correct signed short int cast  : %d\n",(int) numberSShort);
  printf("Test for correct unsigned short int cast: %d\n",(int) numberUShort);
  printf("Test for correct int cast               : %d\n",numberInt);
  printf("Test for correct unsigned int cast      : %X\n",numberUInt);
}

void TestOperators()
{
  TestGreater();
  TestSub();
  TestEqual();
  TestAddVar();
  TestAdd();
  TestMul();
  TestCast();
}