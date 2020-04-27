#ifndef __KILLTOTAL
#define __KILLTOTAL

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class KillTotal;

WX_DECLARE_OBJARRAY(KillTotal,ArrayOfKillTotal);
class KillTotal
{
	public:
		static int Compare(KillTotal *total1,KillTotal *total2);
		KillTotal();
		virtual ~KillTotal();

		bool WriteToDatabase(const char *suffix);
		static wxString SQLTableName(const char *suffix);
		static wxString SQLCreateTable(const char *suffix);
		int itemIndex;
		int playerIndex;
		int kills;
		int deaths;
		int suicides;
		int teamkills;
		
};


#endif
