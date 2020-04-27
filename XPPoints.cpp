// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "XPPoints.h"
#include "GlobalStatistics.h"

XPPoints::XPPoints()
{
}

XPPoints::~XPPoints()
{
}

wxString XPPoints::SQLTableName()
{
	wxString	tableName="xppoints";

	return (tableName);
}

wxString XPPoints::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"playerindex integer,"
				"pointsindex integer,"
				"points float"
			")",
			STRING_TO_CHAR(SQLTableName()));

	return SQL;
}

bool XPPoints::WriteToDatabase(int roundIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);

	SQL.Printf("Insert into %s"
				"(roundindex,playerindex,pointsindex,points)"
				"values('%d','%d','%d','%f')",
				STRING_TO_CHAR(SQLTableName()),
				roundIndex,
				player.actualPlayerIndex,
				pointsIndex,
				points);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}
wxString XPPoints::SQLTotalTableName()
{
	wxString	tableName="xptotalpoints";

	return (tableName);
}

wxString XPPoints::SQLTotalCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"playerindex integer,"
				"pointsindex integer,"
				"points float"
			")",
			STRING_TO_CHAR(SQLTotalTableName()));

	return SQL;
}

bool XPPoints::WriteTotalToDatabase()
{
	wxString	SQL;
	bool		retVal=true;

	SQL.Printf("Insert into %s"
				"(playerindex,pointsindex,points)"
				"values('%d','%d','%f')",
				STRING_TO_CHAR(SQLTotalTableName()),
				playerIndex,
				pointsIndex,
				points);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}
