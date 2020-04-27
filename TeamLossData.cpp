// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "TeamLossData.h"
#include "GlobalStatistics.h"

TeamLossData::TeamLossData()
{
}

TeamLossData::~TeamLossData()
{
}

wxString TeamLossData::SQLTableName()
{
	wxString	retVal="teamlossdata";

	return (retVal);
}

wxString TeamLossData::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"teamlossidx integer,"
				"playerindex integer,"
				"playerclass integer,"
				"playerteam integer"
			")",
			SQLTableName().GetData());

	return SQL;

}

bool TeamLossData::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);
	SQL.Printf("insert into %s"
				"(roundindex,teamlossidx,playerindex,playerclass,playerteam)"
				"values('%d','%d','%d','%d','%d')",
				SQLTableName().GetData(),
				roundIndex,
				itemIndex,
				player.actualPlayerIndex,
				playerClass,
				playerTeam);
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}

