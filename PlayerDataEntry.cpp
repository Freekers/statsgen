// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "PlayerDataEntry.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

PlayerDataEntry::PlayerDataEntry()
{
	playerIndex=-1;
	playerGUID="";
	playerName="";
	playerData="";
}

PlayerDataEntry::~PlayerDataEntry()
{
}

void PlayerDataEntry::ReadFromFile(wxTextFile *fp,int index)
{
	wxString	lineRead;
	wxString	tempString;
	wxString	msg;
	Player		existingPlayer;


	STATSGEN_DEBUG_FUNCTION_START("PlayerDataEntry","ReadFromFile")
	lineRead=fp->GetLine(index);


	tempString=lineRead;
	playerData=tempString.BeforeFirst(',');
	playerGUID=tempString.AfterFirst(',');

	playerName=playerGUID.AfterFirst(',');
	playerGUID=playerGUID.BeforeFirst(',');
	STATSGEN_DEBUG_CODE(
	msg.Printf("name read [%s] [%s]",
				STRING_TO_CHAR(playerGUID),
				STRING_TO_CHAR(playerName));
							)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)

	// Update with any current aliases
	AliasListEntry		alias;

	alias.aliasGUID=playerGUID;
	alias.aliasName=playerName;
	if (globalStatistics.FindAlias(alias))
	{
		playerGUID=alias.primaryGUID;
		playerName=alias.primaryName;
		STATSGEN_DEBUG_CODE(
		msg.Printf("Re-aliased to [%s] [%s]",
				STRING_TO_CHAR(playerGUID),
				STRING_TO_CHAR(playerName));
							)
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
	}

	playerIndex=globalStatistics.FindPlayerIndex(playerName);
	STATSGEN_DEBUG_CODE(
	msg.Printf("playerIndex found %d",playerIndex);
						)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	if (playerIndex==-1)
	{
		// Not in current player list - means it is still in db
		int	dummyPlayerIndex;
		Player	player;
		player.name=playerName;
		if (player.UpdateFromDatabase(dummyPlayerIndex))
		{
			playerIndex=player.actualPlayerIndex;
		}
		else
		{
			playerIndex=-1;
		}
		STATSGEN_DEBUG_CODE(
		msg.Printf("playerIndex in db found %d",playerIndex);
						)
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
	}
	else
	{
		existingPlayer=globalStatistics.playerList.Item(playerIndex);
		if (existingPlayer.actualPlayerIndex >= 0)
		{
			playerIndex=existingPlayer.actualPlayerIndex;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void PlayerDataEntry::WriteToFile(FILE *fp)
{
	STATSGEN_DEBUG_FUNCTION_START("PlayerDataEntry","WriteToFile")
	fprintf(fp,"%s,%s,%s\n",
		STRING_TO_CHAR(playerData),
		STRING_TO_CHAR(playerGUID),
		STRING_TO_CHAR(playerName));
	STATSGEN_DEBUG_FUNCTION_END
}

bool PlayerDataEntry::WriteToDatabase(const char *tableName)
{
	wxString	SQL;
	bool		retVal=true;
	
	SQL.Printf("delete from %s where playerindex=%d",
				tableName,
				playerIndex);
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	if (playerIndex>=0)
	{
		SQL.Printf("insert into %s"
				"(playerindex,playerdata)"
				"values"
				"('%d','%s')",
				tableName,
				playerIndex,
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(playerData))
				);

		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}

	return (retVal);
}

