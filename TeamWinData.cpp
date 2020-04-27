// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "TeamWinData.h"
#include "GlobalStatistics.h"

TeamWinData::TeamWinData()
{
}

TeamWinData::~TeamWinData()
{
}

wxString TeamWinData::SQLTableName()
{
	wxString	retVal="teamwindata";

	return (retVal);
}

wxString TeamWinData::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"teamwinidx integer,"
				"playerindex integer,"
				"playerclass integer,"
				"playerteam integer"
			")",
			SQLTableName().GetData());
	return SQL;
}

bool TeamWinData::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);

	SQL.Printf("insert into %s"
		"(roundindex,teamwinidx,playerindex,playerclass,playerteam)"
		"values"
		"('%d','%d','%d','%d','%d')",
		SQLTableName().GetData(),
		roundIndex,
		itemIndex,
		player.actualPlayerIndex,
		playerClass,
		playerTeam);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}

