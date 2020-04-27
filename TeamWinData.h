#ifndef __TEAMWINDATA
#define __TEAMWINDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class TeamWinData;

WX_DECLARE_OBJARRAY(TeamWinData,ArrayOfTeamWinData);
class TeamWinData
{
	public:
		TeamWinData();
		virtual ~TeamWinData();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int itemIndex);
		
		wxDateTime	winTime;
		int			gameType;
		int			mapName;
		int			playerIndex;
		int			playerClass;
		int			playerTeam;
};


#endif
