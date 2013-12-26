
// CChatRecordDB.cpp : implementation file
// Desc		:	My Own C++ Wrapper for SQLite
// Author	:	Saint Atique
// Dated	:	Dec 12, 2013, SQLite: Dec 22, 2013

#include "stdafx.h"
#include "ChatRecordDB.h"
#include "sqlite3.h"

//#include "dbcommon.h"
//#include <sqlce_oledb.h>
//#include <sqlce_err.h  >


////////////////////////////////////////////////////////////////////////////////
// Function		: CChatRecordDB::Employees()
//
// Description	: Constructor
//
// Notes		: This function will set reference bSuccess according to result of CChatRecordDB
//					This function call precedes opening of call of open
//
////////////////////////////////////////////////////////////////////////////////
CChatRecordDB::CChatRecordDB() : /*m_hWndEmployees(NULL), */
                                       db(NULL),
									   szErrorMsg(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////
// Function: CChatRecordDB::~Employees()
//
// Description: Destructor
//
// Returns: none
//
// Notes:
//
////////////////////////////////////////////////////////////////////////////////
CChatRecordDB::~CChatRecordDB()
{
	sqlite3_close(db);
	db = NULL;
	/*if (szErrorMsg) {
		delete szErrorMsg;
		szErrorMsg = NULL;
	}*/
}

////////////////////////////////////////////////////////////////////////////////
// Function		:	Opens Database, creates if does not exist
//
// Description	:	Initialilly database system is initialized using SQL CE 4.0
//
// Returns		:	False if an error occurs
//
// Notes:
//
////////////////////////////////////////////////////////////////////////////////
BOOL CChatRecordDB::Open()
{
	HANDLE				hFind;							// File handle
	WIN32_FIND_DATA		FindFileData;					// The file structure description  

	// If database exists, open it,
	// Otherwise, create a new database, insert sample data.
	//
	hFind = FindFirstFile(DATABASE_IMDIALERCHAT, &FindFileData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		FindClose(hFind);
		if (OpenDatabase() == FALSE)
			return FALSE;
	}
	else
	{
		// Create IM Dialer database file as it does not exist
		if (CreateDatabase() == FALSE)
			return FALSE;
		// Insert sample data
		if (InsertRows() == FALSE)
			return FALSE;
		// now open the newly created database
		if (OpenDatabase() == FALSE)
			return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Function: CreateDatabase
//
// Description:
//		Create Database File
//		Create user table
//
// Returns: TRUE if succesfull
// ref: 
////////////////////////////////////////////////////////////////////////////////
BOOL CChatRecordDB::CreateDatabase()
{
	// Create Database because it does not exist
	int rc = sqlite3_open16(DATABASE_IMDIALERCHAT, &db);		// ref: http://www.sqlite.org/c3ref/open.html
	if (rc)
	{
		//_tcscpy(szErrorMsg, (LPCTSTR) sqlite3_errmsg16(db));
		szErrorMsg = (LPCTSTR) sqlite3_errmsg16(db);
		sqlite3_close(db);
		return FALSE;
	}

	// Create Table
	LPCSTR sqlCreateTable = "CREATE TABLE UserTable (id INTEGER PRIMARY KEY, value STRING);";
	char *error;
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);		// ref: http://www.sqlite.org/c3ref/exec.html
	if (rc)
	{
		//_tcscpy(szErrorMsg, (LPCTSTR) sqlite3_errmsg16(db));
		szErrorMsg = (LPCTSTR) sqlite3_errmsg16(db);
		sqlite3_free(error);
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Function: OpenDatabase
//
// Description:	Open a connection to database
//
// Returns: NOERROR if succesfull
//
////////////////////////////////////////////////////////////////////////////////
BOOL CChatRecordDB::OpenDatabase()
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Function: Insert Chat History
//
// Description:	Inserts sample data
//
// Returns: NOERROR if succesfull
//
////////////////////////////////////////////////////////////////////////////////
BOOL CChatRecordDB::InsertRows()
{
	   // Execute SQL
	LPSTR error;
	const char *sqlInsert = "INSERT INTO UserTable VALUES(NULL, 'A Value');";
	int rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	if (rc)
	{
		szErrorMsg = (LPCTSTR) sqlite3_errmsg16(db);
		sqlite3_free(error);
		return FALSE;
	}

	return TRUE;
}	
