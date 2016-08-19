#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable: 4477)

void TestClosingCursor()
{
  printf("Test closing the cursor:\n");
  printf("========================\n");

  SQLDatabase dbs;
  // dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      printf("Database opened.\n");
      
      
      for(unsigned ind = 1;ind <= 1500; ++ind)
      {
        SQLQuery query(&dbs);
        CString sql("SELECT COUNT(*) FROM DUAL");
        query.DoSQLStatement(sql);
        printf(".");
        if(ind % 200 == 0)
        {
          printf("\n%d\n",ind);
        }
      }
      printf("\n");
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
  }
  catch(CString& s)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",s);
  }
  catch(...)
  {
    printf("Database ***NOT*** opened for unknown reasons\n");
  }
  long endTime = clock();
  printf("Open  test performed in: %.6f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);

  //////////////////////////////////////////////////////////////////////////
  //
  // CLOSING NOW
  //
  //////////////////////////////////////////////////////////////////////////

  beginTime = clock();
  if(dbs.IsOpen())
  {
    dbs.Close();
  }
  endTime = clock();
  printf("Close test performed in: %.6f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}

