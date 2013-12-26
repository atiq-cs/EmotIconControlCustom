
// CChatRecordDB.h	: header file for database implementation 
// Author			: Saint Atique
// SQLite Adaptation: December 22, 2013

// forward declaration
typedef struct sqlite3 sqlite3;

#pragma once
class CChatRecordDB
{
public:
	CChatRecordDB();
	virtual ~CChatRecordDB();
	BOOL	Open();

private:
	BOOL InitDatabase();
	BOOL OpenDatabase();
	BOOL CreateDatabase();
	BOOL InsertRows();

private:
	LPCTSTR szErrorMsg;
	sqlite3 *db;
};

const WCHAR DATABASE_IMDIALERCHAT[]			= L"IMDialerMain.db";
