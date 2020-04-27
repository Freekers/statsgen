#ifndef __PLAYERDATALIST
#define __PLAYERDATALIST

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/regex.h>

// Statsgen Includes
#include "PlayerDataEntry.h"

class PlayerDataList
{
	public:
		PlayerDataList(const char *filenameConfigKeyIn,
						const char *databaseTableNameIn,
						const char *defaultFilename);
		~PlayerDataList();

		void ReadFromFile();
		void WriteToFile();
		void AddPlayer(wxString &guid,wxString &name,wxString &data);
		int DeletePlayer(wxString &guid,wxString &name);
		wxString	SQLTableName();
		wxString	SQLCreateTable();
		bool		CreateDatabaseTable();
		bool		WriteToDatabase();

		ArrayOfPlayerDataEntry	playerDataList;
		wxString				filenameConfigKey;
		wxString				databaseTableName;
		wxString				defaultFilename;

};


#endif
