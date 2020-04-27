#ifndef __AKALISTENTRY
#define __AKALISTENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>

// Statsgen Includes

class AKAListEntry;

WX_DECLARE_OBJARRAY(AKAListEntry,ArrayOfAKAListEntry);
class AKAListEntry
{
	public:
		AKAListEntry();
		~AKAListEntry();
		int		playerIndex;
		wxString	name;

		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase();
};


#endif
