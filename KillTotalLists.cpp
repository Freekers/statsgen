// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "KillTotalLists.h"
#include "KillTotal.h"
#include "KillData.h"
#include "GlobalStatistics.h"

KillTotalLists::KillTotalLists()
{
}

KillTotalLists::~KillTotalLists()
{
}

void KillTotalLists::Clear()
{
	maps.Clear();
	gameTypes.Clear();
	weapons.Clear();
	locations.Clear();
	teams.Clear();
	classes.Clear();
}

void KillTotalLists::AddKill(
		ArrayOfKillTotal &list,
		int id,
		int killType,
		int playerIndex)
{
	KillTotal		killTotal;
	int				killIndex;
	int				killCount;
	bool			found;

	killCount=list.GetCount();
	found=false;
	for (killIndex=0;killIndex<killCount;killIndex++)
	{
		killTotal=list.Item(killIndex);
		if ((killTotal.itemIndex==id) &&
			(killTotal.playerIndex==playerIndex))
		{
			// Found it - remove it and we can add it
			// at the end
			list.RemoveAt(killIndex);
			found=true;
			break;
		}
	}
	killTotal.playerIndex=playerIndex;
	killTotal.itemIndex=id;
	if (!found)
	{
		killTotal.kills=0;
		killTotal.deaths=0;
		killTotal.teamkills=0;
		killTotal.suicides=0;
	}
	switch (killType)
	{
		case KILL_TYPE_KILL:
			killTotal.kills++;
			break;
		case KILL_TYPE_DEATH:
			killTotal.deaths++;
			break;
		case KILL_TYPE_TEAMKILL:
			killTotal.teamkills++;
			break;
		case KILL_TYPE_SUICIDE:
			killTotal.suicides++;
			break;
	}
	list.Insert(killTotal,0);
}

void KillTotalLists::Add(KillData &killData,
						int playerIndex,
						ArrayOfKillTotal &mapsIn,
						ArrayOfKillTotal &gameTypesIn,
						ArrayOfKillTotal &weaponsIn,
						ArrayOfKillTotal &locationsIn,
						ArrayOfKillTotal &teamsIn,
						ArrayOfKillTotal &classesIn
						)
{
	int	killType;

	killType=killData.KillType(killData.gameType,playerIndex);

	AddKill(mapsIn,
		killData.mapName,
		killType,
		playerIndex);
	AddKill(gameTypesIn,
		killData.gameType,
		killType,
		playerIndex);
	AddKill(weaponsIn,
		killData.playerWeapon,
		killType,
		playerIndex);
	AddKill(locationsIn,
		killData.targetLocation,
		killType,
		playerIndex);
	if (playerIndex==killData.targetIndex)
	{
		AddKill(classesIn,
			killData.targetClass,
			killType,
			playerIndex);
		AddKill(teamsIn,
			killData.targetTeam,
			killType,
			playerIndex);
	}
	else
	{
		AddKill(classesIn,
			killData.playerClass,
			killType,
			playerIndex);
		AddKill(teamsIn,
			killData.playerTeam,
			killType,
			playerIndex);
	}
}

void KillTotalLists::WriteToDatabase(ArrayOfKillTotal &list,const char *suffix)
{
	int			totalCount;
	int			totalIndex;
	KillTotal	killTotal;
	totalCount=list.GetCount();
	for (totalIndex=0;totalIndex<totalCount;totalIndex++)
	{
		killTotal=list.Item(totalIndex);
		killTotal.WriteToDatabase(suffix);
	}
}

void KillTotalLists::WriteToDatabase()
{
	WriteToDatabase(maps,"maps");
	WriteToDatabase(gameTypes,"gametypes");
	WriteToDatabase(weapons,"weapons");
	WriteToDatabase(locations,"locations");
	WriteToDatabase(teams,"teams");
	WriteToDatabase(classes,"classes");
}
void KillTotalLists::Add(
						ArrayOfKillTotal &mapsIn,
						ArrayOfKillTotal &gameTypesIn,
						ArrayOfKillTotal &weaponsIn,
						ArrayOfKillTotal &locationsIn,
						ArrayOfKillTotal &teamsIn,
						ArrayOfKillTotal &classesIn
						)
{
	AppendKills(maps,		mapsIn);
	AppendKills(gameTypes,	gameTypesIn);
	AppendKills(weapons,	weaponsIn);
	AppendKills(locations,	locationsIn);
	AppendKills(teams,		teamsIn);
	AppendKills(classes,	classesIn);
}
void KillTotalLists::AppendKills(
						ArrayOfKillTotal &dest,
						ArrayOfKillTotal &source
						)
{
	int			totalCount;
	int			totalIndex;
	KillTotal	killTotal;
		

	totalCount=source.GetCount();
	for (totalIndex=0;totalIndex<totalCount;totalIndex++)
	{
		killTotal=source.Item(totalIndex);
		dest.Add(killTotal);
	}
}
