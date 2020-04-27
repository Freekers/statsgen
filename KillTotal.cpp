// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "KillTotal.h"
#include "GlobalStatistics.h"

KillTotal::KillTotal()
{
	kills=0;
	deaths=0;
	suicides=0;
	teamkills=0;
}

KillTotal::~KillTotal()
{
}

wxString KillTotal::SQLTableName(const char *suffix)
{
	wxString	tableName;

	tableName.Printf("killtotal%s",suffix);

	return (tableName);
}

wxString KillTotal::SQLCreateTable(const char *suffix)
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"itemindex integer,"
				"playerindex integer,"
				"kills integer,"
				"deaths integer,"
				"suicides integer,"
				"teamkills integer"
			")",
			STRING_TO_CHAR(SQLTableName(suffix)));

	return SQL;
}

bool KillTotal::WriteToDatabase(const char *suffix)
{
	wxString	SQL;
	bool		retVal=true;
	int			actualPlayerIndex;
	Player		player;

	STATSGEN_DEBUG_FUNCTION_START("KillTotal","WriteToDatabase")

	player=globalStatistics.playerList.Item(playerIndex);
	actualPlayerIndex=player.actualPlayerIndex;

	if (actualPlayerIndex<globalStatistics.statsgenDatabase.NextPlayerIndex())
	{

		// Inefficient - move this delete from here
		SQL.Printf("delete from %s where itemindex='%d' and playerindex='%d'",
				STRING_TO_CHAR(SQLTableName(suffix)),
				itemIndex,
				actualPlayerIndex);
		STATSGEN_DEBUG(DEBUG_RARELY,SQL)
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}
	
	SQL.Printf("insert into %s"
				"(itemindex,playerindex,kills,deaths,suicides,"
				"teamkills)"
				"values"
				"('%d','%d','%d','%d','%d',"
				"'%d')",
				STRING_TO_CHAR(SQLTableName(suffix)),
				itemIndex,actualPlayerIndex,kills,deaths,suicides,teamkills);
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	STATSGEN_DEBUG(DEBUG_RARELY,SQL)

	STATSGEN_DEBUG_FUNCTION_END
	return retVal;
}

int KillTotal::Compare(KillTotal *total1,KillTotal *total2)
{
	int retVal;

	retVal=0;
	if (total1->itemIndex<total2->itemIndex)
	{
		retVal=-1;
	}
	else
	{
		if (total1->itemIndex>total2->itemIndex)
		{
			retVal=+1;
		}
		else
		{
			// check player index
			if (total1->playerIndex<total2->playerIndex)
			{
				retVal=-1;
			}
			else
			{
				if (total1->playerIndex>total2->playerIndex)
				{
					retVal=+1;
				}
				else
				{
					retVal=0;
				}
			}
		}
	}

	return (retVal);
}

