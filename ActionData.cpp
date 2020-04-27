// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "ActionData.h"
#include "GlobalStatistics.h"

ActionData::ActionData()
{
}

ActionData::~ActionData()
{
}

wxString ActionData::SQLTableName()
{
	wxString	tableName="actiondata";

	return (tableName);
}

wxString ActionData::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"actionidx integer,"
				"playerindex integer,"
				"playerclass integer,"
				"playerteam integer,"
				"action integer"
			")",
			STRING_TO_CHAR(SQLTableName()));

	return SQL;
}

bool ActionData::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);

	SQL.Printf("Insert into %s"
				"(roundindex,actionidx,playerindex,playerclass,playerteam,action)"
				"values('%d','%d','%d','%d','%d','%d')",
				STRING_TO_CHAR(SQLTableName()),
				roundIndex,
				itemIndex,
				player.actualPlayerIndex,
				playerClass,
				playerTeam,
				action);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}
