#ifndef __ACTIONDATA
#define __ACTIONDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class ActionData;

WX_DECLARE_OBJARRAY(ActionData,ArrayOfActionData);
class ActionData
{
	public:
		ActionData();
		virtual ~ActionData();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int itemIndex);
		
		wxDateTime	actionTime;
		int			gameType;
		int			mapName;
		int			playerIndex;
		int			playerClass;
		int			playerTeam;
		int			action;
		
};


#endif
