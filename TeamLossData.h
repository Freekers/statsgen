#ifndef __TEAMLOSSDATA
#define __TEAMLOSSDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class TeamLossData;

WX_DECLARE_OBJARRAY(TeamLossData,ArrayOfTeamLossData);
class TeamLossData
{
	public:
		TeamLossData();
		virtual ~TeamLossData();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int itemIndex);

		
		wxDateTime	lossTime;
		int			gameType;
		int			mapName;
		int			playerIndex;
		int			playerClass;
		int			playerTeam;
};


#endif
