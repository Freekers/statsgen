// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "AKAListEntry.h"
#include "GlobalStatistics.h"

AKAListEntry::AKAListEntry()
{
}

AKAListEntry::~AKAListEntry()
{
}

wxString AKAListEntry::SQLTableName()
{
	wxString	tableName="akadata";

	return (tableName);
}

wxString AKAListEntry::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"playerindex integer,"
				"%s"
			")",
			SQLTableName().GetData(),
		StatsgenDatabase::StringFieldDefinition("name","akaname",FIELD_WIDTH_PLAYER_NAME).GetData()
	);

	return SQL;
}

bool AKAListEntry::WriteToDatabase()
{
	wxString	SQL;
	bool		retVal=true;
	int			actualPlayerIndex;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);
	actualPlayerIndex=player.actualPlayerIndex;

	if (actualPlayerIndex<globalStatistics.statsgenDatabase.NextPlayerIndex())
	{
		SQL.Printf("delete from %s "
					"where "
					"playerindex='%d' and "
					"name='%s'",
					SQLTableName().GetData(),
					actualPlayerIndex,
					StatsgenDatabase::SafeForInsert(name).GetData());
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
					
	}
	SQL.Printf("Insert into %s"
				"(playerindex,name)"
				"values('%d','%s')",
				SQLTableName().GetData(),
				actualPlayerIndex,
				StatsgenDatabase::SafeForInsert(name).GetData());

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}
