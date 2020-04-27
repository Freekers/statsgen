// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "KillData.h"
#include "GlobalStatistics.h"

KillData::KillData()
{
}

KillData::~KillData()
{
}

char *KillData::KillTypeText()
{
	char *retVal;

	switch (KillType(gameType,playerIndex))
	{
		case KILL_TYPE_KILL:
			retVal=KILL_TYPE_STRING_KILL;
			break;
		case KILL_TYPE_DEATH:
			retVal=KILL_TYPE_STRING_DEATH;
			break;
		case KILL_TYPE_TEAMKILL:
			retVal=KILL_TYPE_STRING_TEAMKILL;
			break;
		case KILL_TYPE_TEAMKILLVICTIM:
			retVal=KILL_TYPE_STRING_TEAMKILLVICTIM;
			break;
		case KILL_TYPE_TEAMSWAP:
			retVal=KILL_TYPE_STRING_TEAMSWAP;
			break;
		case KILL_TYPE_SUICIDE:
			retVal=KILL_TYPE_STRING_SUICIDE;
			break;
		default:
			retVal=KILL_TYPE_STRING_UNKNOWN;
			break;
	}

	return (retVal);
}

KillTypes KillData::KillType(int gameType,int playerIndexIn)
{
	KillTypes	retVal;

	// Make this read from config eventually

	if (playerIndexIn==playerIndex)
	{
		// This is probably a kill
		retVal=KILL_TYPE_KILL;
	}
	else
	{
		// This is probably a death
		retVal=KILL_TYPE_DEATH;
	}
	
	if (playerIndex==targetIndex)
	{
		retVal=KILL_TYPE_SUICIDE;
		if (globalStatistics.IsTeamSwapWeapon(playerWeapon))
		{
			retVal=KILL_TYPE_TEAMSWAP;
		}
	}
	if (playerIndex!=targetIndex)
	{
		if (playerTeam==targetTeam)
		{
			if (!globalStatistics.IsDeathmatchGametype(gameType))
			{
				if (playerIndex==playerIndexIn)
				{
					retVal=KILL_TYPE_TEAMKILL;
				}
				else
				{
					retVal=KILL_TYPE_TEAMKILLVICTIM;
				}
			}
		}
	}

	return (retVal);
}

wxString KillData::SQLTableName()
{
	wxString	tableName="killdata";

	return (tableName);
}

wxString KillData::SQLCreateTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"killidx integer,"
				"playerindex integer,"
				"playerclass integer,"
				"playerteam integer,"
				"playerweapon integer,"
				"playerammo integer,"
				"targetindex integer,"
				"targetclass integer,"
				"targetteam integer,"
				"targetlocation integer,"
				"%s"
			")",
			SQLTableName().GetData(),
		StatsgenDatabase::StringFieldDefinition("killtype","killtype",FIELD_WIDTH_KILL_TYPE).GetData()
	);

	return SQL;
}

bool KillData::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;
	Player		target;

	player=globalStatistics.playerList.Item(playerIndex);
	target=globalStatistics.playerList.Item(targetIndex);
	SQL.Printf("insert into %s"
				"(roundindex,killidx,playerindex,playerclass,playerteam,"
				"playerweapon,playerammo,targetindex,targetclass,"
				"targetteam,targetlocation,killtype)"
				"values"
				"('%d','%d','%d','%d','%d',"
				"'%d','%d','%d','%d',"
				"'%d','%d','%s')",
				SQLTableName().GetData(),
				roundIndex,itemIndex,player.actualPlayerIndex,playerClass,playerTeam,
				playerWeapon,playerAmmo,target.actualPlayerIndex,targetClass,
				targetTeam,targetLocation,KillTypeText());
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}
