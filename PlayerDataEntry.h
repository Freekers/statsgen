#ifndef __PLAYERDATAENTRY
#define __PLAYERDATAENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>
#include <wx/regex.h>

// Statsgen Includes

class PlayerDataEntry;

WX_DECLARE_OBJARRAY(PlayerDataEntry,ArrayOfPlayerDataEntry);
class PlayerDataEntry
{
	public:
		PlayerDataEntry();
		~PlayerDataEntry();
		int		playerIndex;
		wxString	playerName;
		wxString	playerGUID;
		wxString	playerData;
		void ReadFromFile(wxTextFile *fp,int index);
		void WriteToFile(FILE *fp);
		bool WriteToDatabase(const char *tableName);
};


#endif
