#include "stdafx.h"
#include "SQLVariant.h"


void TestGreater()
{
  SQLVariant var1((short)13);
  SQLVariant var2((long) 15);

  if(var1 > var2)
  {
    printf("Error in operator (long > long)\n");
  }
}


void TestOperators()
{
  TestGreater();
}