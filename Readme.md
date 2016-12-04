SQL Components Library
======================

General ODBC components to work with SQL Databases and all gory details therein.
The 'standard' proces to work with these components is very easy and looks something like this:

	// Define a database
	SQLDatabase database;
	
	// Go open the database from some ODBC datasource name (DSN)
	database.Open("serverDSN","user","password");
	
	if(database.IsOpen())
	{
		// Get some meaningful SQL statement :-)
		CString query = "SELECT id,amount FROM invoice";
	
		// Grab a SQLQuery object, and connect to the database
		SQLQuery query(database);
	
		// Go run the statement
		query.DoSQLStatement(sql);
	
		// As long as we have a cursor running
		while(query.GetRecord())
		{
			// Grab a few columns (starting at 1!!)
			int    id     = query[1];
			double amount = query[2]
			
			// Do something with it
			printf("The value of [%d] is: %f\n",id,amount); 
		}
		// Go close it
		// Or don't and just go out of scope
		database.Close();
	}


###DATABASE INDEPENDENT

You can use the following database types through the DBInfoDB class:

- Oracle
- Microsoft SQL Server
- PostgreSQL
- Microsoft Access (Jet engine)
- IBM Informix
- Firebird
- Generic ODBC database

All these databases implement 'standard' ISO SQL, but they all have some
quirky details that are different.
To circumvent all these differences, you can discover the use of the current
database by requesting some method form the DBInfo classes.

To do this, use the methods of "database.GetSQLInfoDB()->SomeMethod()"

There are a host of methods in this class that you can use to find out what
a database option should look like. Is the changing of a column in the database
named 'ALTER' or 'MODIFY' for instance. Or what is the syntax to add a foreign key?


###Dataset oriented

SQLComponents is dataset oriented, as is SQL itself. You can just work with loose SQLQuery 
objects and do your stuf with it, like in the example above. But you can also use the 
**SQLDataSet** object, which is much more powerfull to work with. With this object you can

- Implicitly define queries
- Scroll through result cursors 
- Record multiple user mutations and transactions on the objects
- Save back to the database by simply call "Synchronize()"

The SQLDataSet object will in most instances search for the primary key and foreign keys
for itself and in most cases create the needed INSERT, UPDATE and DELETE statements for itself!

###Powerfull variant class

The **SQLVariant** class is central to this library. In fact, the result values of the 
SQLQuery are all SQLVariants. Just about every standard datatype of the C++ language is 
encapsulated within this class. You can transform every datatype through constructors, cast operators 
and class methods. You can also use the objects of this class directly to do your caculations in,
as most standard mathematical and comparison operators are defined for this class.
Datatypes included are:

- int
- unsigned int
- char
- unsigned char
- const char * and CString
- void *
- short
- unsigned short
- long
- unsigned long
- __int64
- unsigned __int64
- float
- double
- SQLGUID
- SQL\_DATE_STRUCT
- SQL\_TIME_STRUCT
- SQL\_TIMESTAMP_STRUCT
- SQL\_INTERVAL_STRUCT
- SQLDate
- SQLTime
- SQLTimestamp
- SQLInterval
- bcd (Binary coded decimal) 
 
Operators for this class include + * - / % and work for any type to any type,
so you can add say an unsigned short to a double or you can add to strings.

## Transactions

Transactions work by putting a **SQLTransaction** object on the stack. After you processed
everything, you can simply call the "Commit()" method of the transaction object.
Not calling the "Commit()" will do a rollback as the object goes out of scope.
Real safe if some other component does a throw for an error, as the transaction will
also be rolled back. This is how you generally do it:

	// Define a database
	SQLDatabase database;
	
	// Go open the database from some ODBC datasource name (DSN)
	database.Open("serverDSN","user","password");
	
	if(database.IsOpen())
	{
		// Get some meaningful SQL statement :-)
		CString query = "UPDATE invoice SET amount = amount * 2 WHERE id = ?";

		try	
		{
			// Grab a SQLQuery object, and connect to the database
			SQLQuery query(database);
			SQLTransaction trans(database,"update");
	
			// Go run the statement with an embedded parameter
			int id = 3416;
			query.DoSQLStatement(sql,id);

			// Transaction ready, commit it
			trans.Commit();
		}
		catch(CString& er)
		{
			// Catching our error, means the transaction went out of scope
			// meaning it is rolled back automatically
			printf("ERROR: What went wrong: %s\n",er);
		}
		// Go close it
		// Or don't and just go out of scope
		database.Close();
	}

##Unit testing

To make the unit testing framework run, create a 32 bits ODBC datasource name for the 
MS-Access database "basis.accdb" provided in the root folder of this project.
Name that datasource "testing", as it is referenced in the "TestSQL" and "UnitTest"
projects of the solution.

After this datasource has been created, you should be able to run both test sets right away.
The unit test run only in 32 bits, as Microsoft has not decided yet to support 64 bit unit
testing for the C++ language.

The MS-Access database does only work in 32 bits because I had a 32 bits MS-Office installment
on my computer. To make it work in 64 bits, you need to install a 64 bits MS-Office version.
That is the only way to get to a 64 bits MS-Access JET engine driver.
