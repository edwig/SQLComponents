#include "stdafx.h"
#include "SQLVariant.h"
#include "SQLDate.h"

void TestGreater()
{
  SQLVariant var1((short)13);
  SQLVariant var2((long) 15);

  if(var1 > var2)
  {
    printf("Error in operator (long > long)\n");
  }
}

void TestSub()
{
  SQLDate date1("15-10-1959");
  SQLDate date2("20-11-2016");
  SQLVariant var1(&date1);
  SQLVariant var2(&date2);

  SQLVariant var3 = var2 - var1;
  printf("Found interval is: %s\n",var3.GetAsChar());
}

void TestOperators()
{
  TestGreater();
  TestSub();
}