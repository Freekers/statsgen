#ifndef __EXTERNALDATABASE
#define __EXTERNALDATABASE

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

#endif
