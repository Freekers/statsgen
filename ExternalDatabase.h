#ifndef __EXTERNALDATABASE
#define __EXTERNALDATABASE
//#define	EXTERNAL_DB_ENABLED	false
#ifdef EXTERNAL_DB_ENABLED
// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>

#include <mysql/mysql.h>

class ExternalDatabase
{
	public:
		ExternalDatabase();
		virtual ~ExternalDatabase();
		void UpdateFromConfig();
		void TransferFromInternal();
		wxString EscapedString(wxString &escaped);
		void SplitFieldNames(wxString &createSQL,wxArrayString &fieldNames);
		void TransferTableData(wxString &tableName,
								wxArrayString &fieldNames,
								long *currentRowCount);
		wxString	hostname;
		int			port;
		wxString	username;
		wxString	password;
		wxString	databasename;
		bool		enabled;
		bool		createIndexes;
	private:
		bool Connect();
		bool CreateDatabaseInstance();
		bool ExecuteSQL(wxString &SQL);
		long RowCount(wxString &tableName);
		MYSQL	*mySQLHandle;
};
#else
class ExternalDatabase
{
	public:
		ExternalDatabase();
		virtual ~ExternalDatabase();
		void TransferFromInternal();
};
#endif
#endif
